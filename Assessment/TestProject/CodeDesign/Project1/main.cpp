/*
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "Critter.h"
#include "raylib.h"
#include "raymath.h"
#include <random>
#include <time.h>
#include <iostream>
#include "Resource.h"
int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;  //change screen size so the critters can move around better
    
    InitWindow(screenWidth, screenHeight, "AHHAHAHAHAHAHAHA GOOD LUCK WE ARE FAILING AHAHAHHA");

    //--------------------------------------------------------------------------------------

    srand(time(NULL));


  //this is used to stop duplicate
    Resource crittersTexture("res/10.png");
    Resource destroyerTexture("res/9.png");
  


    // create some critters
    const int CRITTER_COUNT = 50; 
    const int MAX_VELOCITY = 80;
    //this will be used to keep track of our sorted array object pool 
  
    Critter critters[CRITTER_COUNT];


    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        // create a random direction vector for the velocity
        Vector2 velocity = { -100+(rand()%200), -100+(rand()%200) };
        // normalize and scale by a random speed
        velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

        // create a critter in a random location
        critters[i].Init(
            { (float)(5+rand() % (screenWidth-10)), (float)(5+(rand() % screenHeight-10)) },
            velocity,
            12);
    }

    //make destroyer critter
    Critter destroyer;
    //Creat a random direction vector for the velocity
    Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
    // normalize and scale by a random speed
    velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);
    //spawn destroyer at random position 
    destroyer.Init(Vector2{ (float)(screenWidth >> 1), (float)(screenHeight >> 1) }, velocity, 20);

    float timer = 1; // wait are these used for ???
    Vector2 nextSpawnPos = destroyer.GetPosition();

    

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        float delta = GetFrameTime();

        // update the destroyer
        destroyer.Update(delta);


        destroyer.CheckCollisionScreen(screenHeight, screenWidth);

        // update the critters
        // (dirty flags will be cleared during update)
        for (int i = 0; i < CRITTER_COUNT; i++)
        {

            if (!critters[i].IsDead()) {


                critters[i].Update(delta);


                critters[i].CheckCollisionScreen(screenHeight, screenWidth);

                // kill any critter touching the destroyer
                // simple circle-to-circle collision check

                float dist = Vector2Distance(critters[i].GetPosition(), destroyer.GetPosition());
                if (dist < critters[i].GetRadius() + destroyer.GetRadius())
                {
                    critters[i].Destroy();
                  
                }
            }
            
        }
                
        // check for critter-on-critter collisions
        for (int i = 0; i < CRITTER_COUNT; i++)
        {   
            for (int j = 1; j < CRITTER_COUNT; j++) {
              
                if (i == j || critters[i].IsDirty()) { break; } 

                // check every critter against every other critter

                float dist = Vector2Distance(critters[i].GetPosition(), critters[j].GetPosition());

                if (dist < critters[i].GetRadius() + critters[j].GetRadius())
                {
                    // collision!
                    // do math to get critters bouncing
                    Vector2 normal = Vector2Normalize(Vector2Subtract(critters[j].GetPosition(), critters[i].GetPosition()));

                    // not even close to real physics, but fine for our needs
                    critters[i].SetVelocity(Vector2Scale(normal, -MAX_VELOCITY));
                    // set the critter to *dirty* so we know not to process any more collisions on it
                    critters[i].SetDirty();

                    // we still want to check for collisions in the case where 1 critter is dirty - so we need a check 
                    // to make sure the other critter is clean before we do the collision response
                    if (critters[j].IsDirty()) { break; }

                    critters[j].SetVelocity(Vector2Scale(normal, MAX_VELOCITY));
                    critters[j].SetDirty();
                }
            }
        }

        timer -= delta;
        if (timer <= 0)
        {
            timer = 1;

             //find any dead critters and spit them out (respawn)
            for (int i = 0; i < CRITTER_COUNT+1; i++)
            {
                if (critters[i].IsDead())
                {
                    Vector2 normal = Vector2Normalize(destroyer.GetVelocity());

                    // get a position behind the destroyer, and far enough away that the critter won't bump into it again
                    Vector2 pos = destroyer.GetPosition();
                    pos = Vector2Add(pos, Vector2Scale(normal, -50));
                    // its pretty ineficient to keep reloading textures. ...if only there was something else we could do 
                    critters[i].Respawn(pos, Vector2Scale(normal, -MAX_VELOCITY));
                    break;
                    
                    
                    
                }
            }
            nextSpawnPos = destroyer.GetPosition();
        }
     
        // Draw
        //==========================================================================
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // draw the critters
      
        for (int i = 0; i < CRITTER_COUNT; i++)
        {
            if (!critters[i].IsDead()) { critters[i].Draw(crittersTexture); }
        }

        destroyer.Draw(destroyerTexture);
        
        // draw the destroyer
       
        DrawFPS(10, 10);
        

        EndDrawing();
     //==============================================================================
    }

    
        UnloadTexture(crittersTexture.GetTexture());
        UnloadTexture(destroyerTexture.GetTexture());
    

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}