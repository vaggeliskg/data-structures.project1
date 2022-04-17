
#include <stdlib.h>

#include "ADTList.h"
#include "state.h"


// Οι ολοκληρωμένες πληροφορίες της κατάστασης του παιχνιδιού.
// Ο τύπος State είναι pointer σε αυτό το struct, αλλά το ίδιο το struct
// δεν είναι ορατό στον χρήστη.

struct state {
	List objects;			// περιέχει στοιχεία Object (Εδαφος / Ελικόπτερα / Πλοία/ Γέφυρες)
	struct state_info info;	// Γενικές πληροφορίες για την κατάσταση του παιχνιδιού
	float speed_factor;		// Πολλαπλασιαστής ταχύτητς (1 = κανονική ταχύτητα, 2 = διπλάσια, κλπ)
};


// Δημιουργεί και επιστρέφει ένα αντικείμενο

static Object create_object(ObjectType type, float x, float y, float width, float height) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	return obj;
}

// Προσθέτει αντικείμενα στην πίστα (η οποία μπορεί να περιέχει ήδη αντικείμενα).
// Τα αντικείμενα ξεκινάνε από y = start_y, και επεκτείνονται προς τα πάνω.
//
// ΠΡΟΣΟΧΗ: όλα τα αντικείμενα έχουν συντεταγμένες x,y σε ένα
// καρτεσιανό επίπεδο.
// - Στο άξονα x το 0 είναι το αριστερό μέρος της πίστας και οι συντεταγμένες
//   μεγαλώνουν προς τα δεξιά. Η πίστα έχει σταθερό πλάτος, οπότε όλες οι
//   συντεταγμένες x είναι ανάμεσα στο 0 και το SCREEN_WIDTH.
//
// - Στον άξονα y το 0 είναι η αρχή της πίστας, και οι συντεταγμένες μεγαλώνουν
//   προς τα _κάτω_. Η πίστα αντιθέτως επεκτείνεται προς τα πάνω, οπότε όλες οι
//   συντεταγμένες των αντικειμένων είναι _αρνητικές_.
//
// Πέρα από τις συντεταγμένες, αποθηκεύουμε και τις διαστάσεις width,height
// κάθε αντικειμένου. Τα x,y,width,height ορίζουν ένα παραλληλόγραμμο, οπότε
// μπορούν να αποθηκευτούν όλα μαζί στο obj->rect τύπου Rectangle (ορίζεται
// στο include/raylib.h). Τα x,y αναφέρονται στην πάνω-αριστερά γωνία του Rectangle.

static void add_objects(State state, float start_y) {
	// Προσθέτουμε BRIDGE_NUM γέφυρες.
	// Στο διάστημα ανάμεσα σε δύο διαδοχικές γέφυρες προσθέτουμε:
	// - Εδαφος, αριστερά και δεξιά της οθόνης (με μεταβαλλόμενο πλάτος).
	// - 3 εχθρούς (ελικόπτερα και πλοία)
	// Τα αντικείμενα έχουν SPACING pixels απόσταση μεταξύ τους.

	for (int i = 0; i < BRIDGE_NUM; i++) {
		// Δημιουργία γέφυρας
		Object bridge = create_object(
			BRIDGE,
			0,								// x στο αριστερό άκρο της οθόνης
			start_y - 4 * (i+1) * SPACING,	// Η γέφυρα i έχει y = 4 * (i+1) * SPACING
			SCREEN_WIDTH,					// Πλάτος ολόκληρη η οθόνη
			20								// Υψος
		);

		// Δημιουργία εδάφους
		Object terrain_left = create_object(
			TERRAIN,
			0,								// Αριστερό έδαφος, x = 0
			bridge->rect.y,					// y ίδιο με την γέφυρα
			rand() % (SCREEN_WIDTH/3),		// Πλάτος επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);
		int width = rand() % (SCREEN_WIDTH/2);
		Object terrain_right = create_object(
			TERRAIN,
			SCREEN_WIDTH - width,			// Δεξί έδαφος, x = <οθόνη> - <πλάτος εδάφους>
			bridge->rect.y,					// y ίδιο με τη γέφυρα
			width,							// Πλάτος, επιλεγμένο τυχαία
			4*SPACING						// Υψος καλύπτει το χώρο ανάμεσα σε 2 γέφυρες
		);

		list_insert_next(state->objects, list_last(state->objects), terrain_left);
		list_insert_next(state->objects, list_last(state->objects), terrain_right);
		list_insert_next(state->objects, list_last(state->objects), bridge);

		// Προσθήκη 3 εχθρών πριν από τη γέφυρα.
		for (int j = 0; j < 3; j++) {
			// Ο πρώτος εχθρός βρίσκεται SPACING pixels κάτω από τη γέφυρα, ο δεύτερος 2*SPACING pixels κάτω από τη γέφυρα, κλπ.
			float y = bridge->rect.y + (j+1)*SPACING;

			Object enemy = rand() % 2 == 0		// Τυχαία επιλογή ανάμεσα σε πλοίο και ελικόπτερο
				? create_object(WARSHIP,    (SCREEN_WIDTH - 83)/2, y, 83, 30)		// οριζόντιο κεντράρισμα
				: create_object(HELICOPTER, (SCREEN_WIDTH - 66)/2, y, 66, 25);
			enemy->forward = rand() % 2 == 0;	// Τυχαία αρχική κατεύθυνση

			list_insert_next(state->objects, list_last(state->objects), enemy);
		}
	}
}

// Δημιουργεί και επιστρέφει την αρχική κατάσταση του παιχνιδιού

State state_create() {
	// Δημιουργία του state
	State state = malloc(sizeof(*state));

	// Γενικές πληροφορίες
	state->info.playing = true;				// Το παιχνίδι ξεκινάει αμέσως
	state->info.paused = false;				// Χωρίς να είναι paused.
	state->info.score = 0;					// Αρχικό σκορ 0
	state->info.missile = NULL;				// Αρχικά δεν υπάρχει πύραυλος
	state->speed_factor = 1;				// Κανονική ταχύτητα

	// Δημιουργία του αεροσκάφους, κεντραρισμένο οριζόντια και με y = 0
	state->info.jet = create_object(JET, (SCREEN_WIDTH - 35)/2,  0, 35, 40);

	// Δημιουργούμε τη λίστα των αντικειμένων, και προσθέτουμε αντικείμενα
	// ξεκινώντας από start_y = 0.
	state->objects = list_create(NULL);
	add_objects(state, 0);

	return state;
}

// Επιστρέφει τις βασικές πληροφορίες του παιχνιδιού στην κατάσταση state

StateInfo state_info(State state) {
	return &state->info;
}

// Επιστρέφει μια λίστα με όλα τα αντικείμενα του παιχνιδιού στην κατάσταση state,
// των οποίων η συντεταγμένη y είναι ανάμεσα στο y_from και y_to.

List state_objects(State state, float y_from, float y_to) {
	List list_objects = list_create(NULL);
	for(ListNode node=list_first(state->objects) ; node!=LIST_EOF ; node=list_next(state->objects, node)) {
		Object obj = list_node_value(state->objects, node);
		if(abs(obj->rect.y) >= abs(y_from) && abs(obj->rect.y) <= abs(y_to) )
			list_insert_next(list_objects, LIST_BOF,obj);
    }
	return list_objects;
}

// Ενημερώνει την κατάσταση state του παιχνιδιού μετά την πάροδο 1 frame.
// Το keys περιέχει τα πλήκτρα τα οποία ήταν πατημένα κατά το frame αυτό.

void state_update(State state, KeyState keys) {
	if(state->info.playing) {
		if(!(keys->up))
			state->info.jet->rect.y -=3 * (state->speed_factor);
		if(keys->up)
			state->info.jet->rect.y -=6 * (state->speed_factor);
		if(keys->down)
			state->info.jet->rect.y -=2 * (state->speed_factor);
		if(keys->right)
			state->info.jet->rect.x +=3 * (state->speed_factor);
		if(keys->left)
			state->info.jet->rect.x -=3 * (state->speed_factor);
		if(keys->space) {
			if((state->info.missile == NULL)) {
			state->info.missile = create_object(MISSILE,state->info.jet->rect.x,state->info.jet->rect.y,5,10);
			}
		}
		if((state->info.missile != NULL)) {
			state->info.missile->rect.y -=10 * (state->speed_factor);
			if((abs(state->info.missile->rect.y) -(abs(state->info.jet->rect.y)) > 800))
				state->info.missile = NULL;
		}
		ListNode last_node = LIST_BOF;

		int state_y_offset = -state->info.jet->rect.y;
		List list_objects = state_objects(state, -state_y_offset + SCREEN_WIDTH, -state_y_offset - 2*SCREEN_WIDTH);
		for(ListNode node=list_first(list_objects) ; node!=LIST_EOF ; node=list_next(list_objects, node)) {
			Object obj = list_node_value(list_objects, node);
			if(obj->type == BRIDGE) {
				if(abs(state->info.jet->rect.y) - abs(obj->rect.y) >= 800) {
					add_objects(state, obj->rect.y);
					//state->speed_factor = 1.3 * state->speed_factor;
				}
			}
			if(obj->forward) {
				if((obj->type == HELICOPTER)) {
					obj->rect.x +=4 * (state->speed_factor);
				}
				else if((obj->type == WARSHIP)) {
					obj->rect.x +=3 * (state->speed_factor);
				}
			}
			else {
				if((obj->type == HELICOPTER)) {
				obj->rect.x -=4 * (state->speed_factor);
				}
				else if((obj->type == WARSHIP)) {
				obj->rect.x -=3 * (state->speed_factor);
				}
			}
			//έλεγχος συγκρούσεων
			if(obj->type == BRIDGE || obj->type == HELICOPTER || obj->type == WARSHIP || obj->type == TERRAIN ) {
				if(CheckCollisionRecs(state->info.jet->rect, obj->rect)) {
					state->info.playing = false;
					return;
				}
				if(obj->type == TERRAIN) {
					if((state->info.missile != NULL)) {
						if(CheckCollisionRecs(state->info.missile->rect, obj->rect)) {
							state->info.missile = NULL;
						}
					}
				}
				//ListNode last_node;
				if(obj->type == BRIDGE || obj->type == HELICOPTER || obj->type == WARSHIP) {
					if((state->info.missile != NULL)) {
						if(CheckCollisionRecs(state->info.missile->rect, obj->rect)) {
							state->info.missile = NULL;
							list_remove_next(list_objects, last_node);
							free(last_node);
							state->info.score +=10;
						}
					}
				}
			}
			last_node=node;	
			if(obj->type == HELICOPTER || obj->type == WARSHIP) {
				Object Enemy = obj;
				if (obj->type == TERRAIN) {
					if(CheckCollisionRecs(obj->rect,Enemy->rect)) {
						if((Enemy->forward)) {
							Enemy->forward = false;
						}
						else if(!(Enemy->forward)) {
							Enemy->forward = true;
						}
					}
				}	
			}
		}
		if(keys->p) {
			state->info.paused = true;
		}	
		if((state->info.paused == true)) {
			if(keys->n) {
				state_update(state,keys);
				return;
			}
			if(keys->p) {
				state->info.paused = false;
			}
		}
	}
	if((state->info.playing == false)) {
		if(keys->enter) {
			state_create();
			//state_update(state, keys);
			return;
		}
	}
	
}


// Καταστρέφει την κατάσταση state ελευθερώνοντας τη δεσμευμένη μνήμη.

//void state_destroy(State state) {
	// Προς υλοποίηση
//}