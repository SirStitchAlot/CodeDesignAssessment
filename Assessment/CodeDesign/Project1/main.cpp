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

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;  //change screen size so the critters can move around better
    
    

    InitWindow(screenWidth, screenHeight, "AHHAHAHAHAHAHAHA GOOD LUCK WE ARE FAILING AHAHAHHA");

    
    //--------------------------------------------------------------------------------------

    srand(time(NULL));


    Critter critters[2]; 

    // create some critters
    const int CRITTER_COUNT = 2; //was 50 just making it a large number for testing //made it max possible to better test out performance
    const int MAX_VELOCITY = 80;
    //this will be used to keep track of our sorted array object pool 
    int  Active_Count = CRITTER_COUNT; //all critters will be active at the start of the game



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
            12, "res/10.png");
    }

    //make destroyer critter
    Critter destroyer;
    //Creat a random direction vector for the velocity
    Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
    // normalize and scale by a random speed
    velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);
    //spawn destroyer at random position 
    destroyer.Init(Vector2{ (float)(screenWidth >> 1), (float)(screenHeight >> 1) }, velocity, 20, "res/9.png");

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
                    std::cout << i << "is being deactivated" << Active_Count - 1 << "is replacing it" << std::endl;
                    critters[i].Destroy();
                    critters[i].DeactivateCritter(critters[Active_Count-1]);
                    std::cout << "arggghhh im dead" << std::endl;
                    Active_Count--;
                    // this would be the perfect time to put the critter into an object pool
                }
            }
            
        }
                
        // check for critter-on-critter collisions
        for (int i = 0; i < Active_Count; i++)
        {
            //check if first critter so we don't need to worry about checking it
            if (critters[i].IsDead()) { break; }
            
         

            
            for (int j = 1; j < Active_Count; j++) {
              
                if (i == j || critters[i].IsDirty()) { break; } //we do >= because if I is higher then j then that critter has already been checked

               
               
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
            for (int i = Active_Count; i < CRITTER_COUNT+1; i++)
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
                    
                    //this will be used for future object pool count just gotta figure it out 
                    Active_Count++;
                }
            }
            nextSpawnPos = destroyer.GetPosition();
        }
     

        // Draw
        //==========================================================================
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // draw the critters
        if (!critters->IsDead()) {
            for (int i = 0; i < CRITTER_COUNT; i++)
            {
                critters[i].Draw();

            }
        }
    
        // draw the destroyer
        // (if you're wondering why it looks a little odd when sometimes critters are destroyed when they're not quite touching the 
        // destroyer, it's because the origin is at the top-left. ...you could fix that!)
        destroyer.Draw();

        DrawFPS(10, 10);
        

        EndDrawing();
     //==============================================================================
    }

    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        UnloadTexture(critters[i].GetTexture());
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}