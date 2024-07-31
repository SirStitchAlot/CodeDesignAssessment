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

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 450;  //change screen size so the critters can move around better

    InitWindow(screenWidth, screenHeight, "AHHAHAHAHAHAHAHA GOOD LUCK WE ARE FAILING AHAHAHHA");

    
    //--------------------------------------------------------------------------------------

    srand(time(NULL));


    Critter critters[50]; 

    // create some critters
    const int CRITTER_COUNT = 50; //was 50 just making it a large number for testing //made it max possible to better test out performance
    const int MAX_VELOCITY = 80;

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
        // check each critter against screen bounds 
        //is this really best way to check collision with the screen
        //we could make this a function for the Critter class it clutters up the loop fucntion 
        //if (destroyer.GetX() < 0) {
        //    destroyer.SetX(0);
        //    destroyer.SetVelocity(Vector2{ -destroyer.GetVelocity().x, destroyer.GetVelocity().y });
        //}            
        //if (destroyer.GetX() > screenWidth) {
        //    destroyer.SetX(screenWidth);
        //    destroyer.SetVelocity(Vector2{ -destroyer.GetVelocity().x, destroyer.GetVelocity().y });
        //}            
        //if (destroyer.GetY() < 0) {
        //    destroyer.SetY(0);
        //    destroyer.SetVelocity(Vector2{ destroyer.GetVelocity().x, -destroyer.GetVelocity().y });
        //}            
        //if (destroyer.GetY() > screenHeight) {
        //    destroyer.SetY(screenHeight);
        //    destroyer.SetVelocity(Vector2{ destroyer.GetVelocity().x, -destroyer.GetVelocity().y });
        //}

        destroyer.CheckCollision(screenHeight, screenWidth);

        // update the critters
        // (dirty flags will be cleared during update)
        for (int i = 0; i < CRITTER_COUNT; i++)
        {
            critters[i].Update(delta);
        
           


                // check each critter against screen bounds
                //again make this a function critter.CheckCollision()
                //if (critters[i].GetX() < 0) {
                //    critters[i].SetX(0);
                //    critters[i].SetVelocity(Vector2{ -critters[i].GetVelocity().x, critters[i].GetVelocity().y });
                //}
                //if (critters[i].GetX() > screenWidth) {
                //    critters[i].SetX(screenWidth);
                //    critters[i].SetVelocity(Vector2{ -critters[i].GetVelocity().x, critters[i].GetVelocity().y });
                //}
                //if (critters[i].GetY() < 0) {
                //    critters[i].SetY(0);
                //    critters[i].SetVelocity(Vector2{ critters[i].GetVelocity().x, -critters[i].GetVelocity().y });
                //}
                //if (critters[i].GetY() > screenHeight) {
                //    critters[i].SetY(screenHeight);
                //    critters[i].SetVelocity(Vector2{ critters[i].GetVelocity().x, -critters[i].GetVelocity().y });
                //}
            critters[i].CheckCollision(screenHeight, screenWidth);
                // kill any critter touching the destroyer
                // simple circle-to-circle collision check

                float dist = Vector2Distance(critters[i].GetPosition(), destroyer.GetPosition());
                if (dist < critters[i].GetRadius() + destroyer.GetRadius())
                {
                    critters[i].Destroy();
                    // this would be the perfect time to put the critter into an object pool //noted
                }
          
        }
                
        // check for critter-on-critter collisions
        for (int i = 0; i < CRITTER_COUNT; i++)
        {            
            for (int j = 0; j < CRITTER_COUNT; j++){
                if (i == j || critters[i].IsDirty()) // note: the other critter (j) could be dirty - that's OK
                    continue;
                // check every critter against every other critter
                float dist = Vector2Distance(critters[i].GetPosition(), critters[j].GetPosition());
                if (dist < critters[i].GetRadius() + critters[j].GetRadius())
                {
                    // collision!
                    // do math to get critters bouncing
                    Vector2 normal = Vector2Normalize( Vector2Subtract(critters[j].GetPosition(), critters[i].GetPosition()));

                    // not even close to real physics, but fine for our needs
                    critters[i].SetVelocity(Vector2Scale(normal, -MAX_VELOCITY));
                    // set the critter to *dirty* so we know not to process any more collisions on it
                    critters[i].SetDirty(); 

                    // we still want to check for collisions in the case where 1 critter is dirty - so we need a check 
                    // to make sure the other critter is clean before we do the collision response
                    if (!critters[j].IsDirty()) {
                        critters[j].SetVelocity(Vector2Scale(normal, MAX_VELOCITY));
                        critters[j].SetDirty();
                    }
                    break;
                }
            }
        }

        timer -= delta;
        if (timer <= 0)
        {
            timer = 1;

            // find any dead critters and spit them out (respawn)
            for (int i = 0; i < CRITTER_COUNT; i++)
            {
                if (critters[i].IsDead())
                {
                    Vector2 normal = Vector2Normalize(destroyer.GetVelocity());

                    // get a position behind the destroyer, and far enough away that the critter won't bump into it again
                    Vector2 pos = destroyer.GetPosition();
                    pos = Vector2Add(pos, Vector2Scale(normal, -50));
                    // its pretty ineficient to keep reloading textures. ...if only there was something else we could do // good to know //lmao could we just do that dumb ass thing where we hand it off screen or what ever????
                    critters[i].Init(pos, Vector2Scale(normal, -MAX_VELOCITY), 12, "res/10.png");
                    break;
                }
            }
            nextSpawnPos = destroyer.GetPosition();
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // draw the critters
        for (int i = 0; i < CRITTER_COUNT; i++)
        {
            critters[i].Draw();
        }
        // draw the destroyer
        // (if you're wondering why it looks a little odd when sometimes critters are destroyed when they're not quite touching the 
        // destroyer, it's because the origin is at the top-left. ...you could fix that!)
        destroyer.Draw();

        DrawFPS(10, 10);
        

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        critters[i].Destroy();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}