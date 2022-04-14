//////////////////////////////////////////////////////////////////
//
// Test για το state.h module
//
//////////////////////////////////////////////////////////////////

#include "acutest.h"			// Απλή βιβλιοθήκη για unit testing
#include "state.h"

void test_state_create() {

	State state = state_create();
	TEST_ASSERT(state != NULL);

	StateInfo info = state_info(state);
	TEST_ASSERT(info != NULL);

	TEST_ASSERT(info->playing);
	TEST_ASSERT(!info->paused);
	TEST_ASSERT(info->score == 0);
	TEST_ASSERT(info->missile == NULL);
	TEST_ASSERT(info->jet->rect.y == 0);
	List state_objects(State state, float y_from, float y_to); 
	List objects = state_objects(state,0,-800);
	TEST_ASSERT(list_size(objects) !=0);
	TEST_ASSERT(objects != NULL);
	TEST_ASSERT(state_objects(state,-5,-800) != NULL);
	TEST_ASSERT(state_objects(state,-5,-3000) != NULL);
	TEST_ASSERT(state_objects(state,-160,-700) != NULL);
}
void test_state_update() {
	State state = state_create();
	TEST_ASSERT(state != NULL && state_info(state) != NULL);

	// Πληροφορίες για τα πλήκτρα (αρχικά κανένα δεν είναι πατημένο)
	struct key_state keys = { false, false, false, false, false, false, false, false };
	
	// Χωρίς κανένα πλήκτρο, το αεροσκάφος μετακινείται 3 pixels πάνω
	Rectangle old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	Rectangle new_rect = state_info(state)->jet->rect;

	TEST_ASSERT( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 3 );

	// Με πατημένο το δεξί βέλος, το αεροσκάφος μετακινείται 6 pixels μπροστά
	keys.up = true;
	old_rect = state_info(state)->jet->rect;
	state_update(state, &keys);
	new_rect = state_info(state)->jet->rect;

	TEST_CHECK( new_rect.x == old_rect.x && new_rect.y == old_rect.y - 6 );

	// Προσθέστε επιπλέον ελέγχους
	keys.space = true;
	state_update(state, &keys);
	Rectangle old_missile = state_info(state)->missile->rect;
	state_update(state, &keys);
	Rectangle new_missile = state_info(state)->missile->rect;
	TEST_ASSERT(new_missile.x == old_missile.x && new_missile.y == old_missile.y -10);

	//έλεγχος συγκρούσεων

	List objects = state_objects(state,0,-5000);
	state_update(state, &keys);
	for(ListNode node=list_first(objects) ; node!=LIST_EOF ; node=list_next(objects, node)) {
		Object obj = list_node_value(objects, node);

		if(obj->type == HELICOPTER || obj->type == BRIDGE || obj->type == WARSHIP ) {
			state_info(state)->missile->rect = obj->rect;
			TEST_ASSERT(CheckCollisionRecs(state_info(state)->missile->rect,obj->rect));
		}
	}		
	state_update(state, &keys);
	TEST_ASSERT(state_info(state)->missile == NULL);
}
// Λίστα με όλα τα tests προς εκτέλεση
TEST_LIST = {
	{ "test_state_create", test_state_create },
	{ "test_state_update", test_state_update },

	{ NULL, NULL } // τερματίζουμε τη λίστα με NULL
};