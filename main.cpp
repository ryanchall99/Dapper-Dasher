#include "raylib.h"

int main() {
    // Window
    const int windowWidth = 512;
    const int windowHeight = 380;

    InitWindow(windowWidth, windowHeight, "Dapper Dasher");
    SetTargetFPS(60);

    // SCARFY VARIABLES
    Texture2D scarfy = LoadTexture("textures/scarfy.png"); // Loading the sprite sheet
    Rectangle scarfyRec = {0, 0, (float)scarfy.width / 6, (float)scarfy.height}; // Creating a rectangle
    Vector2 scarfyPos = {windowWidth/2 - scarfyRec.width/2 , windowHeight - scarfyRec.height}; // Position of the sprite (Character)

    // NEBULA VARIABLES
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    Rectangle nebRec = {0.0, 0.0, (float)nebula.width/8, (float)nebula.height / 8};
    Vector2 nebPos = {windowWidth, windowHeight - nebRec.height}; 

    int nebVel = -600; // Nebula X Velocity (Pixels / s)

    // Animation frame
    int frame = 0;
    const float updateTime = 1.0 / 12.0; // Amount of time before we update the animation frame
    float runningTime = 0;

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
            
            // Update Scarfy Y Position
            scarfyPos.y += velocity * dT; // Update the rectangles Y position with the velocity value.

            // Update Nebula X Position
            nebPos.x += nebVel * dT;
            // if(nebPos.x < 0) {
            //     nebPos.x = windowWidth;
            // }

            // Update Animation Frame
            if(!isInAir) 
            {
                runningTime += dT; // Update runningTime
                if(runningTime >= updateTime) {
                    runningTime = 0;

                    scarfyRec.x = frame * scarfyRec.width;
                    frame++;
                    if(frame > 5) {
                        frame = 0;
                    }
                }
            }

            // Texture Drawing
            DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE); // Scarfy
            DrawTextureRec(nebula, nebRec, nebPos, WHITE); // Nebula

        EndDrawing(); // End of Drawing
    }
    
    UnloadTexture(scarfy); // Unloading Texture
    UnloadTexture(nebula); // Unloading Texture
    CloseWindow(); // Closes Window and unloads OpenGL context

    return 0;
}