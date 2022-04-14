#include "raylib.h"
#include "state.h"
#include "interface.h"


void interface_init() {
    //Αρχικοποίηση του παραθύρου
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
    //SetTargerFPS(60);

}

void interface_close() {
    CloseWindow();
}

//Draw Game
void interface_draw_frame(State state) {
    BeginDrawing();

    //Σχεδιασμός από την αρχή
    ClearBackground(RAYWHITE);

    //StateInfo info = state_info(state);
    int x_offset = (state_info(state)->jet->rect.x - 100);
    int y_offset = -(SCREEN_HEIGHT - 100);
    DrawCircle(state_info(state)->missile->rect.x - x_offset,state_info(state)->missile->rect.y - y_offset, 15, RED);



    EndDrawing();
}