#include "raylib.h"

// Animation Data of texture
struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};


bool isGrounded(AnimData data, int windowHeight) 
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame) 
{
    // update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime)
    {
        data.runningTime = 0;
        // Update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;

        if(data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }

    return data;
}

float updateBgScroll(Texture2D tex, float posX, float scrollSpeed, float deltaTime)
{
    posX -= scrollSpeed * deltaTime;

    DrawTextureEx(tex, {posX, 0}, 0, 2, WHITE);
    DrawTextureEx(tex, {posX + tex.width * 2, 0}, 0, 2, WHITE);

    if(posX <= -tex.width * 2)
    {
        posX = 0;
    }

    return posX;
}

int main() {
    // Window
    int windowDimensions[2] = {512, 380};

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

    const int sizeOfNebulae = 6;
    AnimData nebulae[sizeOfNebulae];;

    for(int i = 0; i < sizeOfNebulae; i++) 
    {
        nebulae[i] = {
            {0.0, 0.0, (float)nebula.width/8, (float)nebula.height/8}, // Rectangle rec
            {(float)windowDimensions[0] + i * 300, (float)windowDimensions[1] - (float)nebula.height/8}, // Vector2 pos
            0, // int frame
            1.0 / 12.0, // float updateTime
            0};
    }

    float finishLine = nebulae[sizeOfNebulae - 1].pos.x + 120;

    int nebVel = -200; // Nebula X Velocity (Pixels / s)

    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX = 0; // Background X Position
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX = 0;
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX = 0;

    // Collision Detection
    bool collision = false;

    while (!WindowShouldClose())
    {
        // Delta Time (Time Since Last Frame)
        const float dT = GetFrameTime();

        BeginDrawing(); // Begin Drawing
            ClearBackground(WHITE); // Clear background to white each frame (Avoids flashing)

            bgX = updateBgScroll(background, bgX, 20, dT);
            mgX = updateBgScroll(midground, mgX, 40, dT);
            fgX = updateBgScroll(foreground, fgX, 80, dT);

            // Ground Check
            if(isGrounded(scarfyData, windowDimensions[1])) 
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

            // Update Nebula X Position (Delta Time)
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                nebulae[i].pos.x += nebVel * dT;
            }

            // Update finish line pos
            finishLine += nebVel * dT;

            // Update Animation Frame
            if(!isInAir)
            {
                scarfyData = updateAnimData(scarfyData, dT, 5);
            }

            for (int i = 0; i < sizeOfNebulae; i++)
            {
                nebulae[i] = updateAnimData(nebulae[i], dT, 7);
            }
            
            // Range based for loop
            for ( AnimData nebula : nebulae )
            {
                // Padding so nebula is actual size
                float padding = 50;
                // Nebulas screen position
                Rectangle nebRec {
                    nebula.pos.x + padding,
                    nebula.pos.y + padding,
                    nebula.rec.width - 2 * padding,
                    nebula.rec.height - 2 * padding
                };
                // Scarfy screen position
                Rectangle scarfyRec {
                    scarfyData.pos.x,
                    scarfyData.pos.y,
                    scarfyData.rec.width,
                    scarfyData.rec.height
                };

                if(CheckCollisionRecs(nebRec, scarfyRec))
                {
                    collision = true;
                }

            }
            
            if(collision)
            {
                // Lose the game
                DrawText("Game Over!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, RED);
            } 
            else if(scarfyData.pos.x >= finishLine)
            {
                // Win the game
                DrawText("You Win!", windowDimensions[0] / 4, windowDimensions[1] / 2, 40, GREEN);
            }
            else
            {
                // Texture Drawing
                DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE); // Scarfy
                // Drawing multiple nebulae
                for(int i = 0; i < sizeOfNebulae; i++) {
                    DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
                }
            }

        EndDrawing(); // End of Drawing
    }
    
    UnloadTexture(scarfy); // Unloading Texture
    UnloadTexture(nebula); // Unloading Texture
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow(); // Closes Window and unloads OpenGL context

    return 0;
}