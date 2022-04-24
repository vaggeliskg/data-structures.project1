#include <stdio.h>
#include "raylib.h"
#include "interface.h"
#include "state.h"

State state;

void update_and_draw() {
    struct key_state keys = {
        .left = IsKeyDown(KEY_LEFT),
        .right = IsKeyDown(KEY_RIGHT),
        .up = IsKeyDown(KEY_UP),
        .down = IsKeyDown(KEY_DOWN),
        .space = IsKeyDown(KEY_SPACE),
        .enter = IsKeyDown(KEY_ENTER),
        .n = IsKeyDown(KEY_N),
        .p = IsKeyDown(KEY_P),
    };
    StateInfo info = state_info(state);
   
       state_update(state, &keys);
       interface_draw_frame(state);
  


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