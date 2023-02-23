#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>

namespace LG
{
	void moveIndex(std::vector<int>& index, std::vector<int>& directions, int direction)
	{
		if (directions[direction] == 1) index[1] = index[1] + 1;
		else if (directions[direction] == 2) index[0] = index[0] + 1;
		else if (directions[direction] == 3) index[1] = index[1] - 1;
		else if (directions[direction] == 4) index[0] = index[0] - 1;
	}

	bool validateIndexValue(int indexValue)
	{
		if (indexValue >= 0 && indexValue < 13) return true;
		else return false;
	}

	void mapRoomCheck(std::vector<int>& index, std::vector<std::vector<int>>& map, std::vector<std::vector<int>>& layout)
	{
		if (validateIndexValue(index[1] + 1)) if (layout[index[0]][index[1] + 1] > 0)
		{
			if (map[index[0]][index[1] + 1] == 0) map[index[0]][index[1] + 1] = 1;
			else if (map[index[0]][index[1] + 1] == 3) map[index[0]][index[1] + 1] = 2;
		}
		if (validateIndexValue(index[0] + 1)) if (layout[index[0] + 1][index[1]] > 0)
		{
			if (map[index[0] + 1][index[1]] == 0) map[index[0] + 1][index[1]] = 1;
			else if(map[index[0] + 1][index[1]] == 3) map[index[0] + 1][index[1]] = 2;
		}
		if (validateIndexValue(index[1] - 1)) if (layout[index[0]][index[1] - 1] > 0)
		{
			if (map[index[0]][index[1] - 1] == 0) map[index[0]][index[1] - 1] = 1;
			else if(map[index[0]][index[1] - 1] == 3) map[index[0]][index[1] - 1] = 2;
		}
		if (validateIndexValue(index[0] - 1)) if (layout[index[0] - 1][index[1]] > 0)
		{
			if (map[index[0] - 1][index[1]] == 0) map[index[0] - 1][index[1]] = 1;
			else if (map[index[0] - 1][index[1]] == 3) map[index[0] - 1][index[1]] = 2;
		}
	}

	std::vector<std::vector<int>> layoutGeneration(int floorNumber = 1)
	{
		//Initialisation

		srand(time(NULL));
		std::vector<std::vector<int>>layout(13, { 0,0,0,0,0,0,0,0,0,0,0,0,0 });
		layout[6][6] = 1;
		std::vector<int>index{6,6};
		std::vector<int>directions{ 1,2,3,4 };
		std::vector<std::vector<int>>generatedRooms;
		bool directionFound = false;

		//Path to exit

		for (int i = 0; i < 5; ++i)
		{
			std::random_shuffle(directions.begin(), directions.end());

			for (int j = 0; j < directions.size() && directionFound == false; ++j)
			{
				moveIndex(index, directions, j);

				if (layout[index[0]][index[1]] == 0)
				{
					int counter = 0;
					if (layout[index[0]][index[1] + 1] > 0) ++counter;
					if (layout[index[0] + 1][index[1]] > 0) ++counter;
					if (layout[index[0]][index[1] - 1] > 0) ++counter;
					if (layout[index[0] - 1][index[1]] > 0) ++counter;
					if (counter < 2)
					{
						layout[index[0]][index[1]] = 2;
						generatedRooms.push_back(index);
						directionFound = true;
					}
					else
					{
						moveIndex(index, directions, 3 - j);
					}
				}
				else
				{
					moveIndex(index, directions, 3 - j);
				}
			}

			directionFound = false;

		}

		//Last uncleared room is replaced with the exit room
		layout[index[0]][index[1]] = 4;
		generatedRooms.pop_back();

		//Extra rooms, randomly placed next to existing rooms
		for (int i = 0; i < 4 + floorNumber; ++i)
		{
			std::random_shuffle(generatedRooms.begin(), generatedRooms.end());

			for (int j = 0; j < generatedRooms.size() && directionFound == false; ++j)
			{
				index = generatedRooms[j];

				std::random_shuffle(directions.begin(), directions.end());

				for (int k = 0; k < directions.size() && directionFound == false; ++k)
				{
					moveIndex(index, directions, k);

					if (validateIndexValue(index[0]) && validateIndexValue(index[1]))
					{
						if (layout[index[0]][index[1]] == 0)
						{
							int counter = 0;
							if (validateIndexValue(index[1] + 1)) if (layout[index[0]][index[1] + 1] > 0) ++counter;
							if (validateIndexValue(index[0] + 1)) if (layout[index[0] + 1][index[1]] > 0) ++counter;
							if (validateIndexValue(index[1] - 1)) if (layout[index[0]][index[1] - 1] > 0) ++counter;
							if (validateIndexValue(index[0] - 1)) if (layout[index[0] - 1][index[1]] > 0) ++counter;
							if (counter < 2)
							{
								layout[index[0]][index[1]] = 2;
								generatedRooms.push_back(index);
								directionFound = true;
							}
							else
							{
								moveIndex(index, directions, 3 - k);
							}
						}
						else
						{
							moveIndex(index, directions, 3 - k);
						}
					}
					else
					{
						moveIndex(index, directions, 3 - k);
					}
				}

			}

			directionFound = false;
		}

		//Last uncleared room is replaced with the item room
		layout[index[0]][index[1]] = 3;

		//lil dev cheat ehehe ehehe :)) devious activities down here for me and me only
		for (int i = 0; i < layout.size(); i++) {
			for (int j = 0; j < layout[i].size(); j++)
				std::cout << layout[i][j] << " ";
			std::cout << std::endl;
		}

		return layout;
	}
}