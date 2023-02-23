#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>

namespace LG
{
	void moveIndex(std::vector<int>& index, std::vector<int> directions, int direction);
	bool validateIndexValue(int indexValue);
	void mapRoomCheck(std::vector<int>&, std::vector<std::vector<int>>&, std::vector<std::vector<int>>&);
	std::vector<std::vector<int>> layoutGeneration(int floorNumber = 1);
}