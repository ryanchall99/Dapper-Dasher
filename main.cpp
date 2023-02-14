#include "raylib.h"

int main() {
    // Window
    const int windowWidth = 512;
    const int windowHeight = 380;

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");
    SetTargetFPS(60);

    // TEXTURE
    Texture2D scarfy = LoadTexture("textures/scarfy.png"); // Loading the sprite sheet
    
    Rectangle scarfyRec; // Creating a rectangle
    scarfyRec.width = scarfy.width / 6; // Width: Spritesheet / 6 (6 Sprites)
    scarfyRec.height = scarfy.height; // Height: Spritesheet Height
    scarfyRec.x = 0; // Left
    scarfyRec.y = 0; // Top

    // Animation frame
    int frame = 0;
    const float updateTime = 1.0 / 12.0; // Amount of time before we update the animation frame
    float runningTime = 0;
    
    Vector2 scarfyPos = {windowWidth/2 - scarfyRec.width/2 , windowHeight - scarfyRec.height}; // Position of the sprite (Character)

    float velocity = 0; // (Pixels / Frame)
    const int jumpVel = -600; // (Pixels / Sec)

    // Acceleration due to gravity (pixels / s) / s
    const float gravity = 1'000;

    bool isInAir = false;

    while (!WindowShouldClose())
    {
        BeginDrawing(); // Begin Drawing
            ClearBackground(WHITE); // Clear background to white each frame (Avoids flashing)
            
            // Delta Time (Time Since Last Frame)
            const float dT = GetFrameTime();

            // Ground Check
            if(scarfyPos.y >= windowHeight - scarfyRec.height) 
            {
                velocity = 0; // Reset Velocity to 0
                isInAir = false; // Set isInAir to false (Grounded)
            } 
            else // If position on Y axis is more than the ground...
            {
                // Applying Gravity
                velocity += gravity * dT; // Applied each frame
                isInAir = true; // Set isInAir to true until grounded
            }

            // Check for jumping
            if(IsKeyPressed(KEY_SPACE) && !isInAir) { velocity += jumpVel; } // If the space key is pressed & not in the air, we apply jump velocity to the overall velocity
            
            // Update Y Position
            scarfyPos.y += velocity * dT; // Update the rectangles Y position with the velocity value.

            // Update Animation Frame
            runningTime += dT; // Update runningTime
            if(runningTime >= updateTime) {
                runningTime = 0;

                scarfyRec.x = frame * scarfyRec.width;
                frame++;
                if(frame > 5) {
                    frame = 0;
                }
            }

            DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        EndDrawing(); // End of Drawing
    }
    
    UnloadTexture(scarfy); // Unloading Texture
    CloseWindow(); // Closes Window and unloads OpenGL context

    return 0;
}