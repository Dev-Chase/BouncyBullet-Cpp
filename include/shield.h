#include "raylib.h"

class Shield {
    public:
        Vector2 pos;
        Vector2 size;
        Vector2 og;
        Vector2 center;
        Vector2 vertices[4];
        float DistanceTweenCenters;
        float rot;
        float DistanceTweenVertices[4];
        float VertexAngles[4];
        Color colour;
        Shield();
        bool RectOverlapsAxis(Vector2 OtherVertices[4], Vector2 Axis);
        bool CircleOverlapsAxis(Vector2 Center, Vector2 Axis);
        bool Shield2RectCol(Vector2 OtherVertices[4]);
};