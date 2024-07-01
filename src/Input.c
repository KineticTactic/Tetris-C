#include "Input.h"

double lastMovementInputTime = 0.0;
double movementSpeed = 0.1;

// Accept double pointer to State to be able to modify the state variable itself
void input_handle(State **state) {
    if (IsKeyPressed(KEY_UP))
        game_rotate_block(*state, 1);

    if (IsKeyDown(KEY_LEFT) &&
        lastMovementInputTime + movementSpeed < GetTime()) {
        game_move_block(*state, -1);
        lastMovementInputTime = GetTime();
    }

    if (IsKeyDown(KEY_RIGHT) &&
        lastMovementInputTime + movementSpeed < GetTime()) {
        game_move_block(*state, 1);
        lastMovementInputTime = GetTime();
    }

    if (IsKeyDown(KEY_DOWN))
        game_set_speed(*state, FAST);
    else
        game_set_speed(*state, NORMAL);

    if (IsKeyPressed(KEY_SPACE))
        game_drop_block(*state);

    if (IsKeyPressed(KEY_R)) {
        game_destroy(*state);
        *state = game_init();
    }

    if (IsKeyPressed(KEY_P))
        (*state)->paused = !(*state)->paused;
}
