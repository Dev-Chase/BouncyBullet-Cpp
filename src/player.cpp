#define _USE_MATH_DEFINES

#include<cmath>
#include "player.h"
#include<iostream>

const int PLAYER_SPEED = 4;

Shield::Shield() : pos(Vector2{0, 0}), size(Vector2{0, 0}), og(Vector2{0, 0}), rot(0), colour(WHITE) {}

Player::Player() : pos(Vector2{0, 0}), size(Vector2{0, 0}), vel(Vector2{0, 0}), colour(WHITE), shield(Shield()) {}

Player::~Player() {
    std::cout << "Player Destroyed" << "\n";
}

float ToDegrees(float n) {
    return n * 180 / M_PI;
}

float ToRadians(float n) {
    return n * M_PI / 180;
}

void Player::reset(Vector2 coords, float width, float height) {
    pos = coords;
    size = Vector2{
        width,
        height
    };
    shield.pos = Vector2{coords.x+12.5F, coords.y+12.5F};
    shield.size = Vector2{55, 15};
    shield.og = Vector2{55/2, 25+10};
}

void Player::update(Vector2 world_offs) {
    // Right Key = 262      D Key = 68
    // Left Key = 263       A Key = 65
    // Up Key = 265         W Key = 87
    // Down Key = 264       S Key = 83
    vel.x = (IsKeyDown(262) || IsKeyDown(68))-(IsKeyDown(263) || IsKeyDown(65));
    vel.y = (IsKeyDown(264) || IsKeyDown(83))-(IsKeyDown(265) || IsKeyDown(87));

    pos.x += vel.x * PLAYER_SPEED;
    pos.y += vel.y * PLAYER_SPEED;

    shield.pos.x += vel.x * PLAYER_SPEED;
    shield.pos.y += vel.y * PLAYER_SPEED;

    float mouse_offs_x = ((shield.pos.x-world_offs.x)-GetMouseX());
    float mouse_offs_y = ((shield.pos.y-world_offs.y)-GetMouseY());

    shield.rot = (ToDegrees(atan2(mouse_offs_y, mouse_offs_x)))+90;
}

void Player::draw(Vector2& offs) {
    DrawRectanglePro(Rectangle{
        shield.pos.x-offs.x,
        shield.pos.y-offs.y,
        shield.size.x,
        shield.size.y
    }, shield.og, shield.rot, shield.colour);
    DrawRectangle(pos.x-offs.x, pos.y-offs.y, size.x, size.y, colour);
}