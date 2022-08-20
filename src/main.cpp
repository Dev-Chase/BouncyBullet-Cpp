#include<iostream>
#include "raylib.h"
#include "player.h"

using namespace std;

enum GameState {
	Start,
	GameOver,
	Setup,
	Play
};

GameState start();
GameState gameover();
GameState setup(Player& player, Vector2& offs);
GameState play(Player& player, Vector2& offs);

const int W = 800;
const int H = 600;

int main(void)
{
	GameState gamestate = Start;

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
        case Start:
            gamestate = start();
            break;
        case GameOver:
            gamestate = gameover();
            break;
        case Setup:
            gamestate = setup(player_ptr, offs_ptr);
            break;
        case Play:
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
        return Setup;
    }

    return Start;
}

GameState gameover() {
    int MeasuredGameoverText = MeasureText("Gameover.", 20);
    int MeasuredRestartText = MeasureText("Press Space or Click to Restart", 20);

    DrawText("Gameover.", W/2-MeasuredGameoverText/2, H/2-30, 20, WHITE);
    DrawText("Press Space or Click to Restart", W/2-MeasuredRestartText/2, H/2-10, 20, WHITE);

    if (IsKeyReleased(32) || IsMouseButtonDown(0)) {
        return Setup;
    }

    return GameOver;
}

GameState setup(Player& player, Vector2& offs) {
    player.reset(Vector2{W/2-12.5, H/2-12.5}, 25, 25);

    return Play;
}


GameState play(Player& player, Vector2& offs) {

    player.update(offs);
    offs = Vector2{player.pos.x-W/2+12.5F, player.pos.y-H/2+12.5F};
    player.draw(offs);

    if (IsKeyPressed(32)){
        return GameOver;
    }

    return Play;
}