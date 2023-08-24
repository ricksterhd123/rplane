#include <iostream>

#include "raylib.h"
#include "raymath.h"
#include "physics.h"

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)
#define FPS (60)
#define WINDOW_TITLE "rplane"

using std::cout;
using std::endl;

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);  // Enable Multi Sampling Anti Aliasing 4x (if available)
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);

    Camera3D camera = {0};
    camera.position = (Vector3){0.0f, 25.0f, -100.0f};
    camera.target = (Vector3){0.0f, 15.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Shader shader = LoadShader(TextFormat(ASSETS_PATH"resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                               TextFormat(ASSETS_PATH"resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));

    // Get some required shader locations
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

    // Ambient light level (some basic lighting)
    int ambientLoc = GetShaderLocation(shader, "ambient");
    float ambient[4] ={ 0.1f, 0.1f, 0.1f, 1.0f };
    SetShaderValue(shader, ambientLoc, &ambient, SHADER_UNIFORM_VEC4);

    World* world = new World();

    ////////////////////////////////////////////
    // Ground
    ////////////////////////////////////////////
    btRigidBody *groundBody = world->addBox(btVector3(0, -50, 0), btVector3(100, 50, 100), 0, 1);
    Model ground = LoadModelFromMesh(GenMeshCube(100, 50, 100));

    ////////////////////////////////////////////
    // Sphere
    ////////////////////////////////////////////
    btRigidBody *sphereBody = world->addSphere(btVector3(0, 10, 0), 5, 10, 1);
    Model sphere = LoadModelFromMesh(GenMeshSphere(5, 10, 10));

    sphere.materials[0].shader = shader;
    ground.materials[0].shader = shader;

    Light sun1 = CreateLight(LIGHT_POINT, (Vector3){ 50, 50, 0 }, Vector3Zero(), WHITE, shader);
    Light sun2 = CreateLight(LIGHT_POINT, (Vector3){ -50, 50, 0 }, Vector3Zero(), WHITE, shader);
    Light sun3 = CreateLight(LIGHT_POINT, (Vector3){ 0, 50, 50 }, Vector3Zero(), WHITE, shader);
    Light sun4 = CreateLight(LIGHT_POINT, (Vector3){ 0, 50, -50 }, Vector3Zero(), WHITE, shader);

    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        world->step(FPS);

        if (IsKeyDown(KEY_Z)) {
            // sphereBody->getWorldTransform().setOrigin((btVector3){0, 50, 0});
            sphereBody->setLinearVelocity((btVector3){0, 10, 0});
            sphereBody->setActivationState(ACTIVE_TAG);
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawFPS(10, 10);

        BeginMode3D(camera);
        {
            btTransform sphereTransform = sphereBody->getWorldTransform();
            btVector3 sphereOrigin = sphereTransform.getOrigin();
            Vector3 spherePosition = {sphereOrigin.x(), sphereOrigin.y(), sphereOrigin.z()};
            
            btTransform groundTransform = groundBody->getWorldTransform();
            btVector3 groundOrigin = groundTransform.getOrigin();
            Vector3 groundPosition = {0, -25, 0};

            DrawModel(sphere, spherePosition, 1, GREEN);
            DrawModel(ground, groundPosition, 1, WHITE);

            // DrawGrid(100, 1.0f);
        }
        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    UnloadModel(sphere);
    UnloadModel(ground);
    UnloadShader(shader);

    delete world;

    return 0;
}
