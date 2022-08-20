#include "raylib.h"
#include "shield.h"

class Player {
    public:
        Shield shield;
        Vector2 pos;
        Vector2 size;
        Vector2 vel;
        Color colour;
        Player();
        ~Player();
        void reset(Vector2 coords, float width, float height);
        void draw(Vector2& offs);
        void update(Vector2 world_offs);
};