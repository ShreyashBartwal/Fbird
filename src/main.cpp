#include "raylib.h"
#include <vector>
#include <stdlib.h>

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

//-- Obstacle Struct --
struct Obstacle
{
    Rectangle topPipe;
    Rectangle bottomPipe;
    float speed;
    bool passed;
};

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "FBird");
    SetTargetFPS(60);

    GameScreen currentScreen = TITLE;
    Bird player;

    std::vector<Obstacle> pipes;
    float pipeSpeed = 4.0f;
    int pipeWidth = 60;
    
    int framesCounter = 0;
    int score = 0;

    while (!WindowShouldClose())
    {
        switch (currentScreen)
        {
        case TITLE:
            if (IsKeyPressed(KEY_SPACE))
                currentScreen = GAMEPLAY;
            break;

        case GAMEPLAY:
        {
            player.Update();
            framesCounter++;
            // 1. Spawn pipes every 90 frames
            if (framesCounter >= 90)
            {
                framesCounter = 0;
                int gapSize = GetRandomValue(100, 200);
                // Calculate random heights for the gap
                int minHeight = 50;
                int maxHeight = screenHeight - gapSize - minHeight;
                int topPipeHeight = GetRandomValue(minHeight, maxHeight);
                int bottomPipeHeight = screenHeight - gapSize - topPipeHeight;

                // Create Pipe object and add to vector
                Obstacle newPipe;
                newPipe.topPipe = {(float)screenWidth, 0, (float)pipeWidth, (float)topPipeHeight};
                newPipe.bottomPipe = {(float)screenWidth, (float)(topPipeHeight + gapSize), (float)pipeWidth, (float)(screenHeight - topPipeHeight - gapSize)};
                newPipe.speed = pipeSpeed;
                newPipe.passed = false;

                pipes.push_back(newPipe);
            }
            // Logic to move pipes and check for collisions
            bool hitDetected = false;
            for (auto &pipe : pipes)
            {
                pipe.topPipe.x -= pipe.speed;
                pipe.bottomPipe.x -= pipe.speed;

                // Check for collision with player
                if (CheckCollisionCircleRec(player.position, player.radius, pipe.topPipe) ||
                    CheckCollisionCircleRec(player.position, player.radius, pipe.bottomPipe))
                {
                    hitDetected = true;
                    break;
                }
                if (player.position.x > pipe.topPipe.x + pipeWidth && !pipe.passed)
                {
                    score++;
                    pipe.passed = true;
                }
            }
            // Remove pipes that have gone off screen
            // The coordinates (x, y) represent the top-left corner of the rectangle.
            if (!pipes.empty() && pipes.front().topPipe.x < -pipeWidth)
            {
                pipes.erase(pipes.begin());
            }
            // Floor Ceiling collision
            if (player.position.y >= screenHeight - player.radius || player.position.y <= player.radius)
            {
                hitDetected = true;
            }
            // If a hit is detected, switch to ENDING screen
            if (hitDetected)
            {
                currentScreen = ENDING;
            }
            break;
        }

        case ENDING:
            if (IsKeyPressed(KEY_SPACE))
            {
                player.Reset();
                pipes.clear();
                framesCounter = 0;
                currentScreen = GAMEPLAY;
                score = 0;
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
            // Draw Pipes
            for (auto &pipe : pipes)
            {
                DrawRectangleRec(pipe.topPipe, GREEN);
                DrawRectangleRec(pipe.bottomPipe, GREEN);
            }
            // Draw Score
            DrawText(TextFormat("Score: %i", score), 20, 20, 20, DARKGRAY);
            // Draw Player
            player.Draw();
            break;
        case ENDING:
            for (auto &pipe : pipes)
            {
                DrawRectangleRec(pipe.topPipe, GREEN);
                DrawRectangleRec(pipe.bottomPipe, GREEN);
            }
            player.Draw();

            // Draw Game Over text
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));
            DrawText(TextFormat("Final Score: %i", score), 20, 20, 40, DARKGRAY);
            DrawText("GAME OVER", screenWidth / 2 - 100, screenHeight / 2 - 40, 40, MAROON);
            DrawText("Press SPACE to Restart", screenWidth / 2 - 110, screenHeight / 2 + 20, 20, LIGHTGRAY);
            break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}