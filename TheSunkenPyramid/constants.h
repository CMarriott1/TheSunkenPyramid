#pragma once

struct Dim2Di
{
	int x,y;
};

namespace GC
{
	//Directions
	const int Up = 0;
	const int Right = 1;
	const int Down = 2;
	const int Left = 3;

	//Speeds
	const float PlayerSpeed = 180.f;
	const float PlayerProjectileSpeed = 270.f;
	const float PlayerFiringSpeed = 0.75f;

	const float BatSpeed = 120.f;
	const float EnemyProjectileSpeed = 270.f;
	const float EnemyFiringSpeed = 1.f;

	//Radii (for collisions)
	const int CharRadius = 16;
	const int ProjRadius = 2;

	//Firing effects
	const std::vector<int>StandardShot{ 0,0 };
	const std::vector<int>DoubleShotLeft{ -8,0 };
	const std::vector<int>DoubleShotRight{ 8,0 };
	const std::vector<int>RightShot{ 0,1 };
	const std::vector<int>BackShot{ 0,2 };
	const std::vector<int>LeftShot{ 0,3 };

	//Enemy Spawning
	const int EnemySpawn = 464; //Gamesize minus 2 walls and character length
	const int MinSpawnDist = 24; //Wall plus half char sprite (origin is centre)
	const int EnemySpawnHalf = 216; //Half gamesize - 1 wall and character length. Ironically not half

	//Misc
	const int WallSize = 8;
	const int GameSize = 512;

	const Dim2Di ScreenCentre{ 256,384 };
	const Dim2Di FloorCentre{ 6,6 };
	const Dim2Di WindowSize{512, 640};
	const Dim2Di LowerBounds{ 0,128 };
}