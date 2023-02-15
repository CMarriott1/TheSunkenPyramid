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

	//Misc
	const int PlayerRadius = 16;
	const Dim2Di ScreenCentre{ 256,384 };
	const Dim2Di FloorCentre{ 6,6 };
	const Dim2Di WindowSize{512, 640};
	const Dim2Di LowerBounds{ 0,128 };
}