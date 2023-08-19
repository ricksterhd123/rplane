#include <iostream>

#include "raylib.h"
#include "raymath.h"
#include "physics.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)
#define FPS (60)
#define WINDOW_TITLE "rplane"

using std::cout;
using std::endl;

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 100.0f, -50.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    World* world = new World();

    ////////////////////////////////////////////
    // Ground
    ////////////////////////////////////////////

    btRigidBody *ground = world->addBox(btVector3(0, -55, 0), btVector3(100, 50, 100), 0, 1);

    ////////////////////////////////////////////
    // Sphere
    ////////////////////////////////////////////

    btRigidBody *sphere = world->addSphere(btVector3(0, 10, 0), 5, 10, 1);

    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_FREE);

        world->step(FPS);

        if (IsKeyDown(KEY_Z)) {
            sphere->getWorldTransform().setOrigin((btVector3){0, 50, 0});
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        {
            btTransform trans = sphere->getWorldTransform();
            btVector3 origin = trans.getOrigin();

            Vector3 spherePosition = {origin.x(), origin.y(), origin.z()};
            // cout << spherePosition.x << ", " << spherePosition.y << ", " << spherePosition.z << endl;

            DrawSphere(spherePosition, 5, GREEN);
            DrawPlane((Vector3){0.0f, -5.0f, 0.0f}, (Vector2){100.0f, 100.0f}, GRAY);
            DrawGrid(20, 1.0f);
        }
        EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    delete world;

    return 0;
}
