#include "pch.h"
#include "CppUnitTest.h"
#include "../Project1/Critter.h"
#include "../Project1/Resource.h"
#include "../Project1/OldCritter.h"
#include "../Project1/Critter.cpp"
#include "../Project1/OldCritter.cpp"
#include <stdio.h>
#include <chrono>
#include "raylib.h"
#include "iostream"
#include "fstream"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	std::ofstream MyFile("TestingResults.txt");

	TEST_CLASS(UnitTest1)
	{
	public:
		//compares optmisation to see if new method of storing textures is actaully smaller
		TEST_METHOD(MemoryUsage_ResourceManagment)
		{
			//loading one texture to make sure this is fair
			Resource NewMethodTexture("res / 10.png");
			//build an array of this object to since this is where this method is supposed to improve
			Critter NewMethod[50];

			int NewMethodSize = sizeof(NewMethod) + sizeof(NewMethodTexture);


			OldCritter OldMethod[50];

			int OldMethodSize = sizeof(OldMethod);

			MyFile << "this is the Memory Usage test results" << std::endl;
			MyFile << "Results are shown in bytes" << std::endl;
			MyFile << "NewMethod: " << NewMethodSize << std::endl;
			MyFile << "OldMethod: " << OldMethodSize << std::endl;


			Assert::IsTrue(NewMethodSize < OldMethodSize);




		}



		//this is testing the old respawn/destroy method 
		TEST_METHOD(RespawnMethod_ObjectPooling){

			MyFile << "this is testing the speed of my object pooling" << std::endl;
			
			//this gets the critters ready for the test and should not be included in the time

			int screenWidth = 800;
			int screenHeight = 450;

			const int MAX_VELOCITY = 80;

			Critter NEWcritters[50];

			for (int i = 0; i < 50; i++)
			{

				Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };

				velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

				NEWcritters[i].Init(
					{ (float)(5 + rand() % (screenWidth - 10)), (float)(5 + (rand() % screenHeight - 10)) },
					velocity,
					12);
			}

			//make destroyer critter
			Critter NEWdestroyer;
			//Creat a random direction vector for the velocity
			Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
			// normalize and scale by a random speed
			velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);
			//spawn destroyer at random position 
			NEWdestroyer.Init(Vector2{ (float)(screenWidth >> 1), (float)(screenHeight >> 1) }, velocity, 20);

			
			//starts the timer for new method
			std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

			
			//destroy all the critters
			for (int i = 0; i < 50; i++)
			{
				NEWcritters[i].Destroy();
			}


			//find dead critters and respawn them
			for (int i = 0; i < 50; i++)
			{
				if (NEWcritters[i].IsDead())
				{
					Vector2 normal = Vector2Normalize(NEWdestroyer.GetVelocity());

					// get a position behind the destroyer, and far enough away that the critter won't bump into it again
					Vector2 pos = NEWdestroyer.GetPosition();
					pos = Vector2Add(pos, Vector2Scale(normal, -50));
					// its pretty ineficient to keep reloading textures. ...if only there was something else we could do 
					NEWcritters[i].Respawn(pos, Vector2Scale(normal, -MAX_VELOCITY));
					break;
				}
			}

			//time of finishing the operation for new method
			std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		

			//comapres the two times for the new method
			std::chrono::duration<double> NewMethod_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

			MyFile << "New Method: " << NewMethod_time_span.count() << std::endl;

//=================================================================================
		
			//gets the OLD critter method ready (we also reuse some variables from the first test)

			OldCritter OLDcritters[50];

			for (int i = 0; i < 50; i++)
			{
				// create a random direction vector for the velocity
				Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
				// normalize and scale by a random speed
				velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

				// create a critter in a random location
				OLDcritters[i].Init(
					{ (float)(5 + rand() % (screenWidth - 10)), (float)(5 + (rand() % screenHeight - 10)) },
					velocity,
					12, "res/10.png");
			}

			OldCritter OLDdestroyer;
			velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);
			OLDdestroyer.Init(Vector2{ (float)(screenWidth >> 1), (float)(screenHeight >> 1) }, velocity, 20, "res/9.png");

			//start test for old method
			std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();


			for (int i = 0; i < 50; i++)
			{
				OLDcritters[i].Destroy();
			}

			for  (int i =0; i <50; i++)
			{
				for (int i = 0; i < 50; i++)
				{
					if (OLDcritters[i].IsDead())
					{
						Vector2 normal = Vector2Normalize(OLDdestroyer.GetVelocity());

						// get a position behind the destroyer, and far enough away that the critter won't bump into it again
						Vector2 pos = OLDdestroyer.GetPosition();
						pos = Vector2Add(pos, Vector2Scale(normal, -50));
						// its pretty ineficient to keep reloading textures. ...if only there was something else we could do
						OLDcritters[i].Init(pos, Vector2Scale(normal, -MAX_VELOCITY), 12, "res/10.png");
						break;
					}
				}
				
			}

			//time of finishing old method
			std::chrono::steady_clock::time_point t4 = std::chrono::steady_clock::now();


			//compares start time and finish time for old method
			std::chrono::duration<double> OldMethod_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t4 - t3);


			
			
			MyFile << "Old Method: " << OldMethod_time_span.count() << std::endl;


			//sees if the new method is new
			Assert::IsTrue(NewMethod_time_span < OldMethod_time_span);

		}

		//this will test the speed of the new drawing method using our psudo resource manager
		TEST_METHOD(DrawingMethodSpeed_ResourceManagment) {

			int screenWidth = 800;
			int screenHeight = 450;

			const int MAX_VELOCITY = 80;

			std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

			Resource NEW_CritterTexture("res / 10.png");
			Resource NEW_DestroyerTexture("res/9.png");

			Critter NEW_CritterMethod[50];

			for (int i = 0; i < 50; i++)
			{
				// create a random direction vector for the velocity
				Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
				// normalize and scale by a random speed
				velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

				// create a critter in a random location
				NEW_CritterMethod[i].Init(
					{ (float)(5 + rand() % (screenWidth - 10)), (float)(5 + (rand() % screenHeight - 10)) },
					velocity,
					12);
			}

			//make destroyer critter
			Critter NEW_destroyerMethod;
			//Creat a random direction vector for the velocity
			Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
			// normalize and scale by a random speed
			velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);
			//spawn destroyer at random position 
			NEW_destroyerMethod.Init(Vector2{ (float)(screenWidth >> 1), (float)(screenHeight >> 1) }, velocity, 20);

			for (int i = 0; i < 50; i++)
			{
				if (!NEW_CritterMethod[i].IsDead()) { NEW_CritterMethod[i].Draw(NEW_CritterTexture); }
			}

			NEW_destroyerMethod.Draw(NEW_DestroyerTexture);



			std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();

			std::chrono::duration<double> NewMethod_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);


		//=====================================================================================



		
			std::chrono::steady_clock::time_point t3 = std::chrono::steady_clock::now();

			OldCritter OLD_crittersMethod[50];

			for (int i = 0; i < 50; i++)
			{
				// create a random direction vector for the velocity
				Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
				// normalize and scale by a random speed
				velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

				// create a critter in a random location
				OLD_crittersMethod[i].Init(
					{ (float)(5 + rand() % (screenWidth - 10)), (float)(5 + (rand() % screenHeight - 10)) },
					velocity,
					12, "res/10.png");
			}

			OldCritter OLD_DestroyerMethod;

			OLD_DestroyerMethod.Init(Vector2{ (float)(screenWidth >> 1), (float)(screenHeight >> 1) }, velocity, 20, "res/9.png");


			for (int i = 0; i < 50; i++)
			{
				OLD_crittersMethod[i].Draw();
			}

			OLD_DestroyerMethod.Draw();

			std::chrono::steady_clock::time_point t4 = std::chrono::steady_clock::now();



			std::chrono::duration<double> OldMethod_time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t4 - t3);


			
			MyFile << "This is testing the speed of my resource managment" << std::endl;
			MyFile << "New Method: " << NewMethod_time_span.count() << std::endl;
			MyFile << "Old Method: " << OldMethod_time_span.count() << std::endl;

			Assert::IsTrue(NewMethod_time_span < OldMethod_time_span);
		}

	};












}
