#include "raylib.h"
#include "state.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>

//assets

Texture jet_img;
Texture helir_img;
Texture helil_img;
Texture warshipr_img;
Texture warshipl_img;
Texture wall_img;

void interface_init() {
    //Αρχικοποίηση του παραθύρου
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
    SetTargetFPS(60);
    jet_img = LoadTextureFromImage(LoadImage("assets/jet.png"));
    helir_img = LoadTextureFromImage(LoadImage("assets/helir.png"));
    helil_img = LoadTextureFromImage(LoadImage("assets/helil.png"));
    warshipr_img = LoadTextureFromImage(LoadImage("assets/warshipr.png"));
    warshipl_img = LoadTextureFromImage(LoadImage("assets/warshipl.png"));
    wall_img = LoadTextureFromImage(LoadImage("assets/wall.png"));
}

void interface_close() {
    CloseWindow();
}


//Draw Game
void interface_draw_frame(State state) {
    BeginDrawing();
    ClearBackground(BLUE);


    StateInfo info = state_info(state);
    
// offset για μετάβαση από συντεταγμένες state σε οθόνης
    int x_offset = 0;
    int y_offset = -SCREEN_HEIGHT + 100;

//κάμερα για να ακολουθεί το τζετ στον άξονα ψ
    Camera2D camera = { 0 };
    int x_target = (SCREEN_WIDTH - 35)/2;
    camera.target = (Vector2) {x_target, info->jet->rect.y};
    camera.offset = (Vector2) {x_target , y_offset + 700};
    camera.zoom = 1.0f;

    BeginMode2D(camera);
    //σχεδίαση αντικειμένων + αλλαγή εικόνας ώστε τα αντικείμενα να κοιτάνε προς την κατεύθυνση που πάνε
    DrawTexture(jet_img,info->jet->rect.x - x_offset , info->jet->rect.y - y_offset,BLUE);
    if(info->missile != NULL)
       DrawRectangle(info->missile->rect.x - x_offset , info->missile->rect.y - y_offset, info->missile->rect.width, info->missile->rect.height, RED); 
    int state_objects_offset = -info->jet->rect.y;
    List objects = state_objects(state, -state_objects_offset + 100, -state_objects_offset - 5*SCREEN_WIDTH);

    for(ListNode node=list_first(objects) ; node!=LIST_EOF ; node=list_next(objects, node)) {
		Object obj = list_node_value(objects, node);
        if(obj->type== TERRAIN) {
            DrawRectangle(obj->rect.x - x_offset , obj->rect.y - y_offset, obj->rect.width, obj->rect.height, DARKGREEN);
        }
        if(obj->type == HELICOPTER) {
            if(obj->forward) {
                DrawTexture(helir_img,obj->rect.x,obj->rect.y-y_offset,BLUE);
            }
            else DrawTexture(helil_img,obj->rect.x,obj->rect.y-y_offset,BLUE);
        }
         if(obj->type == BRIDGE) {
            DrawTexture(wall_img,obj->rect.x,obj->rect.y-y_offset,WHITE);
        }
        if(obj->type == WARSHIP) {
            if(obj->forward) {
            DrawTexture(warshipr_img,obj->rect.x,obj->rect.y-y_offset,BLUE);
            }
            else DrawTexture(warshipl_img,obj->rect.x,obj->rect.y-y_offset,BLUE);
        }   
    }
    //σχεδιασμός FPS και σκορ
    DrawText(TextFormat("%04i",info->score), 20, info->jet->rect.y +110, 20, WHITE);
    DrawFPS(SCREEN_WIDTH-80, info->jet->rect.y +110);



    if((!info->playing)) {
        DrawText(
            "PRESS [ENTER] TO PLAY AGAIN ",
             GetScreenWidth() / 2 - MeasureText( "PRESS [ENTER] TO PLAY AGAIN ", 20) / 2,
            info->jet->rect.y + GetScreenHeight() / 2 - 50,20, WHITE
        );
    }
    if(info->paused) {
        DrawText(
        "PRESS [P] TO PLAY  ",
             GetScreenWidth() / 2 - MeasureText( "PRESS [P] TO PLAY  ", 20) / 2,
            info->jet->rect.y + GetScreenHeight() / 2 - 50,20, WHITE
        );
    }
    EndMode2D();
    EndDrawing();
}