#define _USE_MATH_DEFINES

#include<cmath>
#include "player.h"
#include<iostream>

const int PLAYER_SPEED = 4;
const int W = 800;
const int H = 600;

Vector2 RectGetInterval(Vector2 Vertices[4], Vector2 Axis);

Shield::Shield() : pos(Vector2{0, 0}), size(Vector2{0, 0}), og(Vector2{0, 0}), rot(0), colour(WHITE), center(Vector2{0, 0}), DistanceTweenCenters(0)  {}

Player::Player() : pos(Vector2{0, 0}), size(Vector2{0, 0}), vel(Vector2{0, 0}), shield(Shield()), colour(WHITE), angle(0.0F) {}

Player::~Player() {
    std::cout << "Player Destroyed" << "\n";
}

float ToDegrees(float n) {
    return n * 180 / M_PI;
}

float ToRadians(float n) {
    return n * M_PI / 180;
}

float GetDistance(Vector2 from, Vector2 to) {
    if ((from.x-to.x) != 0 && (from.y-to.y) != 0) {
        return (from.x-to.x)/(cos(atan((from.y-to.y)/(from.x-to.x))));
    } else if ((from.x-to.x) == 0 && (from.y-to.y) != 0) {
        return from.y-to.y;
    }else if ((from.y-to.y) == 0 && (from.x-to.x) != 0) {
        return from.x-to.x;
    } else {
        return 0;
    }
}

float GetDistancePyth(Vector2 from, Vector2 to) {
    return sqrt((pow(abs(from.x-to.x), 2))+(pow(abs(from.y-to.y), 2)));
}

void Player::reset(Vector2 coords, float width, float height) {
    this->pos = coords;
    this->size = Vector2{
        width,
        height
    };
    this->shield.pos = Vector2{coords.x+12.5F, coords.y+12.5F};
    this->shield.size = Vector2{55, 15};
    this->shield.og = Vector2{55/2, 25+12.5};

    this->shield.center = Vector2{W/2, H/2-height-5};
    this->shield.DistanceTweenCenters = H/2-(H/2-height-5);
    this->shield.vertices[0] = Vector2{W/2-55/2, H/2-height/2-15-10}; //TOPLEFT
    this->shield.vertices[1] = Vector2{W/2+55/2, H/2-height/2-15-10}; //TOPRIGHT
    this->shield.vertices[2] = Vector2{W/2-55/2, H/2-height/2-10}; // BOTTOMLEFT
    this->shield.vertices[3] = Vector2{W/2+55/2, H/2-height/2-10}; // BOTTOMRIGHT

    this->shield.DistanceTweenVertices[0] = GetDistancePyth(Vector2{W/2, H/2}, shield.vertices[0]);
    this->shield.DistanceTweenVertices[1] = GetDistancePyth(Vector2{W/2, H/2}, shield.vertices[1]);
    this->shield.DistanceTweenVertices[2] = GetDistancePyth(Vector2{W/2, H/2}, shield.vertices[2]);
    this->shield.DistanceTweenVertices[3] = GetDistancePyth(Vector2{W/2, H/2}, shield.vertices[3]);

    this->shield.VertexAngles[0] = 35.7539;
    this->shield.VertexAngles[1] = -35.7539;
    this->shield.VertexAngles[2] = 50.1944;
    this->shield.VertexAngles[3] = -50.1944;
}

void Player::move(Vector2 Vels) {
    pos.x += Vels.x;
    pos.y += Vels.y;
    shield.pos.x += Vels.x;
    shield.pos.y += Vels.y;
    for (int i = 0; i < 4; i++) {
        shield.vertices[i] = Vector2{
            (shield.pos.x) - ((sin(ToRadians(shield.VertexAngles[i]-shield.rot)))*shield.DistanceTweenVertices[i]),
            (shield.pos.y) - ((cos(ToRadians(shield.VertexAngles[i]-shield.rot)))*shield.DistanceTweenVertices[i])
        };
    }
}

void Player::rotate(float WorldOffsX, float WorldOffsY) {
    float mouse_offs_x = ((shield.pos.x-WorldOffsX)-GetMouseX());
    float mouse_offs_y = ((shield.pos.y-WorldOffsY)-GetMouseY());

    shield.rot = (ToDegrees(atan2(mouse_offs_y, mouse_offs_x)))+90;

    for (int i = 0; i < 4; i++) {
        shield.vertices[i] = Vector2{
            (shield.pos.x) - ((sin(ToRadians(shield.VertexAngles[i]-shield.rot)))*shield.DistanceTweenVertices[i]),
            (shield.pos.y) - ((cos(ToRadians(shield.VertexAngles[i]-shield.rot)))*shield.DistanceTweenVertices[i])
        };
    }
}

void Player::update(Vector2 world_offs, Rectangle Walls[], Vector2 WallsVertices[][4], int SizeofWalls) {
    // Right Key = 262      D Key = 68
    // Left Key = 263       A Key = 65
    // Up Key = 265         W Key = 87
    // Down Key = 264       S Key = 83
    // Changing Velocity of Player based on Keyboard Input
    vel.x = (IsKeyDown(262) || IsKeyDown(68))-(IsKeyDown(263) || IsKeyDown(65));
    vel.y = (IsKeyDown(264) || IsKeyDown(83))-(IsKeyDown(265) || IsKeyDown(87));

    // Applying Vertical Movement
    this->move(Vector2{0, vel.y*PLAYER_SPEED});

    // Vertical Collision Checks with Walls
    for (int i = 0; i < SizeofWalls; i++) {
        if (CheckCollisionRecs(Rectangle{pos.x, pos.y, size.x, size.y}, Walls[i]) || shield.Shield2RectCol(WallsVertices[i])) {
            if (vel.y != 0) {
                float WhichWay = 0;
                if (vel.y > 0) {
                    vel.y = 0;
                    WhichWay = -1;
                } else if (vel.y < 0) {
                    vel.y = 0;
                    WhichWay = 1;   
                }
                while (CheckCollisionRecs(Rectangle{pos.x, pos.y, size.x, size.y}, Walls[i]) || shield.Shield2RectCol(WallsVertices[i])) {
                    this->move(Vector2{0, 0.1F*WhichWay});
                }
            }
        }
    }


    // Applying Horizontal Collision
    this->move(Vector2{vel.x*PLAYER_SPEED, 0});

    // Horizontal Collision Checks with Walls
    for (int i = 0; i < SizeofWalls; i++) {
        if (CheckCollisionRecs(Rectangle{pos.x, pos.y, size.x, size.y}, Walls[i]) || shield.Shield2RectCol(WallsVertices[i])) {
            if (vel.x != 0) {
                float WhichWay = 0;
                if (vel.x > 0) {
                    WhichWay = -1;
                } else if (vel.x < 0) {
                    WhichWay = 1;   
                }
                while (CheckCollisionRecs(Rectangle{pos.x, pos.y, size.x, size.y}, Walls[i]) || shield.Shield2RectCol(WallsVertices[i])) {
                    this->move(Vector2{0.1F*WhichWay, 0});
                }
            }
        }
    }
    
    this->rotate(world_offs.x, world_offs.y);
}

void Player::draw(Vector2& offs) {
    DrawRectanglePro(Rectangle{
        shield.pos.x-offs.x,
        shield.pos.y-offs.y,
        shield.size.x,
        shield.size.y
    }, shield.og, shield.rot, shield.colour);
    DrawRectangle(pos.x-offs.x, pos.y-offs.y, size.x, size.y, colour);


    shield.center = Vector2{
        pos.x+12.5F - ((sin(ToRadians(-shield.rot)))*shield.DistanceTweenCenters),
        pos.y+12.5F - ((cos(ToRadians(-shield.rot)))*shield.DistanceTweenCenters)
    };

    DrawCircle(shield.center.x-offs.x, shield.center.y-offs.y, 2, GREEN);
    for (int i = 0; i < 4; i++) {
        DrawCircle(shield.vertices[i].x-offs.x, shield.vertices[i].y-offs.y, 2, GREEN);
    }
}

bool Player::WithinCollisionRange(Vector2 OtherCenter, float Rad1, float Rad2) {
    if (abs(GetDistance(Vector2{pos.x+size.x/2, pos.y+size.y/2}, OtherCenter)) < Rad1+Rad2) {
        return true;
    }
    return false;
}

bool Shield::Shield2RectCol(Vector2 OtherVertices[4]) {
    Vector2 AxesToTest[4] = {
        Vector2{0, 1},
        Vector2{1, 0}
    };
    
    for (int i = 0; i < 2; i++) {
        if (!(this->RectOverlapsAxis(OtherVertices, AxesToTest[i]))) {
            return false;
        }
    }
    return true;
}

bool Shield::RectOverlapsAxis(Vector2 OtherVertices[4], Vector2 Axis) {
    Vector2 interval1 = RectGetInterval(vertices, Axis);
    Vector2 interval2 = RectGetInterval(OtherVertices, Axis);
    return ((interval2.x <= interval1.y) && (interval1.x <= interval2.y));
}

float Dot(Vector2 V1, Vector2 V2) {
    return (V1.x*V2.x)+(V1.y*V2.y);
}

bool Shield::CircleOverlapsAxis(Vector2 Center, Vector2 Axis) {
    Vector2 interval1 = RectGetInterval(vertices, Axis);
    Vector2 interval2 = Vector2{Dot(Axis, Center), Dot(Axis, Center)};
    return ((interval2.x <= interval1.y) && (interval1.x <= interval2.y));
}

Vector2 RectGetInterval(Vector2 vertices[4], Vector2 Axis) {
    Vector2 result = Vector2{0, 0};

    result.x = Dot(Axis, vertices[0]);
    result.y = result.x;
    
    for (int i = 0;i < 4; i++) {
        float projection = Dot(Axis, vertices[i]);
        if (projection < result.x) {
            result.x = projection;
        }
        if (projection > result.y) {
            result.y = projection;
        }
    }

    return result;
}