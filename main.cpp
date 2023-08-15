#include <raylib.h>
#include <math.h>

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)

#define AREA (500)
#define CIRCLE_SIZE (25)

#define WINDOW_TITLE "rplane"

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (int i = 0; i < 100; i++) {
            int jitterX = GetRandomValue(-AREA, AREA);
            int jitterY = GetRandomValue(-AREA, AREA);

            unsigned char r = GetRandomValue(0, 255);
            unsigned char g = GetRandomValue(0, 255);
            unsigned char b = GetRandomValue(0, 255);

            Color color = { r, g, b, 255 };

            DrawCircle(SCREEN_WIDTH / 2 + jitterX, SCREEN_HEIGHT / 2 + jitterY, 25 + sin(GetTime()) * 75, color);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
