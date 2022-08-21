#include<iostream>
#include "raylib.h"
#include "player.h"

using namespace std;

enum class GameState {
	Start,
	GameOver,
	Setup,
	Play
};

GameState start();
GameState gameover();
GameState setup(Player& player);
GameState play(Player& player, Vector2& offs);

const int W = 800;
const int H = 600;
const Rectangle TEST = Rectangle{0, 0, 50, 50};

int main(void)
{
	GameState gamestate = GameState::Start;

    Vector2 offs;
    Vector2& offs_ptr = offs;

	Player player;
    Player& player_ptr = player;

    InitWindow(W, H, "Bouncy Bullet");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();

		ClearBackground(BLACK);

        switch (gamestate)
        {
        case GameState::Start:
            gamestate = start();
            break;
        case GameState::GameOver:
            gamestate = gameover();
            break;
        case GameState::Setup:
            gamestate = setup(player_ptr);
            break;
        case GameState::Play:
            DrawRectangle(TEST.x-offs.x, TEST.y-offs.y, TEST.width, TEST.height, RED);
            gamestate = play(player_ptr, offs_ptr);
            break;
        default:
            break;
        }

		DrawText("Testing", W/2-offs.x, H/2-offs.y, 20, WHITE);
        EndDrawing();
    }
    CloseWindow();
}

GameState start() {
    int MeasuredStartText = MeasureText("Press Space or Click to Start", 20);
    DrawText("Press Space or Click to Start", W/2-MeasuredStartText/2, H/2-10, 20, WHITE);

    if (IsKeyReleased(32) || IsMouseButtonDown(0)) {
        return GameState::Setup;
    }

    return GameState::Start;
}

GameState gameover() {
    int MeasuredGameoverText = MeasureText("Gameover.", 20);
    int MeasuredRestartText = MeasureText("Press Space or Click to Restart", 20);

    DrawText("Gameover.", W/2-MeasuredGameoverText/2, H/2-30, 20, WHITE);
    DrawText("Press Space or Click to Restart", W/2-MeasuredRestartText/2, H/2-10, 20, WHITE);

    if (IsKeyReleased(32) || IsMouseButtonDown(0)) {
        return GameState::Setup;
    }

    return GameState::GameOver;
}

GameState setup(Player& player) {
    player.reset(Vector2{W/2-12.5, H/2-12.5}, 25, 25);

    return GameState::Play;
}


GameState play(Player& player, Vector2& offs) {
    Rectangle RECS[] = {TEST};
    Vector2 REC_VERTICES[][4] = {
        {
            Vector2{0, 0},
            Vector2{0, 50},
            Vector2{50, 50},
            Vector2{50, 0}
        }
    };
    player.update(offs, RECS, REC_VERTICES, *(&RECS + 1) - RECS);
    offs = Vector2{player.pos.x-W/2+12.5F, player.pos.y-H/2+12.5F};

    // if (player.shield.Shield2RectCol(TEST_RECT_VERTICES)) {
        // Collision Code with Shield Goes here
        // cout << "Hit Shield" << endl;
    // } 
    // else {
    //     cout << "Didn't Hit Shield" << endl;
    // }


    player.draw(offs);

    if (IsKeyPressed(32)){
        return GameState::GameOver;
    }

    return GameState::Play;
}