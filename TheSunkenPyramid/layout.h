#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>

namespace LG
{
	void movePointer(std::vector<int>& pointer, std::vector<int> directions, int index);
	bool validatePointerValue(int pointerValue);
	std::vector<std::vector<int>> layoutGeneration(int floorNumber = 1);
}