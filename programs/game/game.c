#include <stdio.h>
#include "raylib.h"
#include "interface.h"
#include "state.h"
#include <stdlib.h>

State state;

void update_and_draw() {
    struct key_state keys = {
        .left = IsKeyDown(KEY_LEFT),
        .right = IsKeyDown(KEY_RIGHT),
        .up = IsKeyDown(KEY_UP),
        .down = IsKeyDown(KEY_DOWN),
        .space = IsKeyPressed(KEY_SPACE),
        .enter = IsKeyDown(KEY_ENTER),
        .n = IsKeyPressed(KEY_N),
        .p = IsKeyPressed(KEY_P),
    };
    //pause and start
    StateInfo info = state_info(state);
    if(info->playing && info->paused == false) {
       state_update(state, &keys);
       interface_draw_frame(state);
    }
    else if(info->playing && info->paused == true) {
      if(keys.p == true) {
        state_update(state, &keys);
        interface_draw_frame(state);
      }
      else if(keys.p == false)
        interface_draw_frame(state);
      if(keys.n == true) {
        state_update(state, &keys);
        interface_draw_frame(state);
      }
    }
    //start over if game is over
    if(info->playing == false) {
      if(keys.enter == true)  {
        state = state_create();
        state_update(state, &keys);
        interface_draw_frame(state);
        return;
      }
      else if(keys.enter == false) {
        interface_draw_frame(state);
        return;
      }
    }
}

int main() {
    state = state_create();
    interface_init();
    start_main_loop(update_and_draw);

    interface_close();

    return  0;
}