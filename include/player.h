#include "raylib.h"
#include "shield.h"

class Player {
    public:
        Shield shield;
        Vector2 pos;
        Vector2 size;
        Vector2 vel;
        Color colour;
        float angle;
        Player();
        ~Player();
        void reset(Vector2 coords, float width, float height);
        void draw(Vector2& offs);
        void update(Vector2 world_offs);
        bool WithinCollisionRange(Vector2 OtherCenter, float Rad1, float Rad2);
        bool OverlapsAxis(Vector2 OtherVertices[4], Vector2 Axis);
        bool Shield2RectCol(Vector2 OtherVertices[4]);
};