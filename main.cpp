#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};


int main() {
    // Window
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher");
    SetTargetFPS(60);

    // SCARFY VARIABLES
    Texture2D scarfy = LoadTexture("textures/scarfy.png"); // Loading the sprite sheet
    AnimData scarfyData{
        {0, 0, (float)scarfy.width / 6, (float)scarfy.height},
        {windowDimensions[0]/2 - scarfyData.rec.width/2 , (float)windowDimensions[1] - (float)scarfy.height},
        0,
        1.0 / 12.0,
        0};

    float velocity = 0; // (Pixels / Frame)
    const int jumpVel = -600; // (Pixels / Sec)

    // Acceleration due to gravity (pixels / s) / s
    const float gravity = 1'000;

    bool isInAir = false;

    // NEBULA VARIABLES
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    AnimData nebData{
        {0.0, 0.0, (float)nebula.width/8, (float)nebula.height/8}, // Rectangle rec
        {(float)windowDimensions[0], (float)windowDimensions[1] - (float)nebula.height/8}, // Vector2 pos
        0, // int frame
        1.0 / 12.0, // float updateTime
        0}; // float runningTime

    AnimData neb2Data{
        {0.0, 0.0, (float)nebula.width/8, (float)nebula.height / 8},
        {(float)windowDimensions[0] + 300, windowDimensions[1] - (float)nebula.height/8},
        0,
        1.0 / 12.0,
        0};

    int nebVel = -200; // Nebula X Velocity (Pixels / s)

    while (!WindowShouldClose())
    {
        BeginDrawing(); // Begin Drawing
            ClearBackground(WHITE); // Clear background to white each frame (Avoids flashing)
            
            // Delta Time (Time Since Last Frame)
            const float dT = GetFrameTime();

            // Ground Check
            if(scarfyData.pos.y >= windowDimensions[1] - scarfyData.rec.height) 
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
            scarfyData.pos.y += velocity * dT; // Update the rectangles Y position with the velocity value.

            // Update Nebula X Position
            nebData.pos.x += nebVel * dT;
            neb2Data.pos.x += nebVel * dT;

            // Update Animation Frame
            if(!isInAir){

                scarfyData.runningTime += dT; // Update runningTime
                if(scarfyData.runningTime >= scarfyData.updateTime) {
                    scarfyData.runningTime = 0;
                    
                    scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;

                    scarfyData.frame++;
                    
                    if(scarfyData.frame > 5) {
                        scarfyData.frame = 0;
                    }
                }
            }

            nebData.runningTime += dT;
            if(nebData.runningTime >= nebData.updateTime) {
                nebData.runningTime = 0;

                nebData.rec.x = nebData.frame * nebData.rec.width;
                nebData.frame++;
                if(nebData.frame > 7) {
                    nebData.frame = 0;
                }
            }

            neb2Data.runningTime += dT;
            if(neb2Data.runningTime >= neb2Data.updateTime) {
                neb2Data.runningTime = 0;

                neb2Data.rec.x = neb2Data.frame * neb2Data.rec.width;
                neb2Data.frame++;
                if(neb2Data.frame > 7) {
                    neb2Data.frame = 0;
                }
            }

            // Texture Drawing
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE); // Scarfy
            DrawTextureRec(nebula, nebData.rec, nebData.pos, WHITE); // Nebula
            DrawTextureRec(nebula, neb2Data.rec, neb2Data.pos, WHITE); // Nebula

        EndDrawing(); // End of Drawing
    }
    
    UnloadTexture(scarfy); // Unloading Texture
    UnloadTexture(nebula); // Unloading Texture
    CloseWindow(); // Closes Window and unloads OpenGL context

    return 0;
}