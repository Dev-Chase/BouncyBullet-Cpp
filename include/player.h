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
        void move(Vector2 Vels);
        void rotate(float WorldOffsX, float WorldOffsY);
        void update(Vector2 world_offs, Rectangle Walls[], Vector2 WallsVertices[][4], int SizeofWalls);
        bool WithinCollisionRange(Vector2 OtherCenter, float Rad1, float Rad2);
        bool CollidedRectangle(Rectangle OtherRect);
};