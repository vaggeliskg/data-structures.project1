#include "ADTSet.h"
#include <stdio.h>
#include "set_utils.h"

// Επιστρέφει την μοναδική τιμή του set που είναι ισοδύναμη με value,
// ή αν δεν υπάρχει, την μικρότερη τιμή του set που είναι μεγαλύτερη
// από value. Αν δεν υπάρχει καμία τότε επιστρέφει NULL.

Pointer set_find_eq_or_greater(Set set, Pointer value) {
    SetNode node = set_find_node(set , value);
    if(set_size(set) == 0) {
        return NULL;
    }
    else if(node == SET_EOF) {
        set_insert(set, value);
        SetNode temp_node = set_find_node(set, value);
        node = set_next(set, temp_node);
        set_remove(set, value);
        return set_node_value(set, node);
    }
    else if(node != SET_EOF) {
        return value;
    }
    return NULL;
} 

// Επιστρέφει την μοναδική τιμή του set που είναι ισοδύναμη με value,
// ή αν δεν υπάρχει, την μεγαλύτερη τιμή του set που είναι μικρότερη
// από value. Αν δεν υπάρχει καμία τότε επιστρέφει NULL.

Pointer set_find_eq_or_smaller(Set set, Pointer value);