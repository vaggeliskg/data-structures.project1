#include <stdio.h>
#include "raylib.h"
#include "interface.h"
#include "state.h"

State state;
typedef struct key_state* KeyState;
KeyState keys;

void update_and_draw() {
    state_update(state, keys);
    interface_draw_frame(state);
}

int main() {
    state = state_create();
    interface_init();

    start_main_loop(update_and_draw);

    interface_close();

    return  0;
}