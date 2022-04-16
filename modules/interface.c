#include "raylib.h"
#include "state.h"
#include "interface.h"
#include <stdio.h>
#include <stdlib.h>

void interface_init() {
    //Αρχικοποίηση του παραθύρου
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game");
    //SetTargerFPS(60);

}

void interface_close() {
    CloseWindow();
}

static Object create_object(ObjectType type, float x, float y, float width, float height) {
	Object obj = malloc(sizeof(*obj));
	obj->type = type;
	obj->rect.x = x;
	obj->rect.y = y;
	obj->rect.width = width;
	obj->rect.height = height;
	return obj;
}

//Draw Game
void interface_draw_frame(State state) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    StateInfo info = state_info(state);

    //int x_offset = (state_info(state)->jet->rect.x - 200 );
    int x_offset = 0;
    int y_offset = -(SCREEN_HEIGHT - 100);

    Camera2D camera = { 0 };
    camera.target = (Vector2) {(SCREEN_WIDTH - 35)/2, info->jet->rect.y};
    camera.offset = (Vector2) {(SCREEN_WIDTH - 35)/2 , y_offset + 700};
    camera.zoom = 1.0f;

    BeginMode2D(camera);


    DrawCircle(info->jet->rect.x - x_offset , info->jet->rect.y - y_offset, 15, RED);

    //printf("x: %f  y: %f\n", info->jet->rect.x, info->jet->rect.y);
    
    int state_y_offset = -info->jet->rect.y;
    List objects = state_objects(state, -state_y_offset + SCREEN_WIDTH, -state_y_offset - 2*SCREEN_WIDTH);

    for(ListNode node=list_first(objects) ; node!=LIST_EOF ; node=list_next(objects, node)) {
		Object obj = list_node_value(objects, node);
        DrawRectangle(obj->rect.x - x_offset , obj->rect.y - y_offset, obj->rect.width, obj->rect.height, BLUE);
        if(obj->type == HELICOPTER) {
            Object temp_obj = create_object(HELICOPTER, obj->rect.x, obj->rect.y - y_offset, obj->rect.width, obj->rect.height);
            DrawRectangleRec(temp_obj->rect, YELLOW);
        }
         if(obj->type == BRIDGE) {
            DrawRectangle(obj->rect.x - x_offset , obj->rect.y - y_offset, obj->rect.width, obj->rect.height, BROWN);
        }
        if(obj->type == WARSHIP) {
             DrawRectangle(obj->rect.x - x_offset , obj->rect.y - y_offset, obj->rect.width, obj->rect.height, PINK);
        }   
    }
    if((!info->playing)) {
        DrawText(
            "PRESS [ENTER] TO PLAY AGAIN ",
             GetScreenWidth() / 2 - MeasureText( "PRESS [ENTER] TO PLAY AGAIN ", 20) / 2,
             GetScreenHeight() / 2 - 50,20, GRAY
        );
    }
    //EndMode2D();
    EndDrawing();
}