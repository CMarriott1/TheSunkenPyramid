#pragma once

struct Dim2Di
{
	int x,y;
};

namespace GC
{
	const Dim2Di ScreenCentre{ 256,384 };
	const Dim2Di FloorCentre{ 5,5 };
	const Dim2Di WindowSize{512, 640};
	const Dim2Di LowerBounds{ 0,128 };

	const char EscapeKey(27);
}