#include "raylib.h"

//-- Game states --
enum GameScreen
{
    TITLE,
    GAMEPLAY,
    ENDING
};

//-- Bird Class --
class Bird
{
public:
    Vector2 position;
    float velocity;
    float gravity;
    float jumpForce;
    float radius;

    Bird()
    {
        Reset();
        gravity = 0.3f;
        jumpForce = 9.0f;
        radius = 15.0f;
    }
    void Reset()
    {
        position = {200, 225};
        velocity = 0.0f;
    }
    void Update()
    {
        velocity += gravity;
        position.y += velocity;

        if (IsKeyPressed(KEY_SPACE))
        {
            velocity -= jumpForce;
        }
    }
    void Draw()
    {
        DrawCircleV(position, radius, MAROON);
    }
};

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "FBird");
    SetTargetFPS(60);

    GameScreen currentScreen = TITLE;
    Bird player;

    while (!WindowShouldClose())
    {
        switch (currentScreen)
        {
        case TITLE:
            if (IsKeyPressed(KEY_SPACE))
                currentScreen = GAMEPLAY;
            break;
        case GAMEPLAY:
            player.Update();
            if (player.position.y >= screenHeight - player.radius)
            {
                currentScreen = ENDING;
            }
            break;
        case ENDING:
            if (IsKeyPressed(KEY_SPACE))
            {
                player.Reset();
                currentScreen = GAMEPLAY;
            }
            break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentScreen)
        {
        case TITLE:
            DrawText("FBird", 20, 20, 40, DARKGRAY);
            DrawText("Press SPACE to start", 20, 80, 20, BLUE);
            break;
        case GAMEPLAY:
            player.Draw();
            break;
        case ENDING:
            DrawText("Game Over!", 20, 20, 40, RED);
            DrawText("Press SPACE to start", 20, 80, 20, BLUE);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}