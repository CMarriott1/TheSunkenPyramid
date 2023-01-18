#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>

void movePointer(std::vector<int>& pointer, std::vector<int> directions, int index)
{
		if (directions[index] == 1) pointer[1] = pointer[1] + 1;
		else if (directions[index] == 2) pointer[0] = pointer[0] + 1;
		else if (directions[index] == 3) pointer[1] = pointer[1] - 1;
		else if (directions[index] == 4) pointer[0] = pointer[0] - 1;
}

bool validatePointerValue(int pointerValue)
{
	if (pointerValue >= 0 && pointerValue < 13) return true;
	else return false;
}

int main()
{
	//Initialisation
	srand(time(NULL));
	std::vector<std::vector<int>>layout(13,{0,0,0,0,0,0,0,0,0,0,0,0,0});
	layout[5][5] = 1;
	std::vector<int>pointer(2, 5);
	std::vector<int>directions{ 1,2,3,4 };
	std::vector<std::vector<int>>generatedRooms;
	bool directionFound = false;

	//Path to end

	for (int i = 0; i < 5; ++i)
	{
		std::random_shuffle(directions.begin(), directions.end());

		for(int j = 0; j < directions.size() && directionFound == false; ++j)
		{
			movePointer(pointer, directions, j);

			if (layout[pointer[0]][pointer[1]] == 0)
			{
				int counter = 0;
				if (layout[pointer[0]][pointer[1] + 1] > 0) ++counter;
				if (layout[pointer[0] + 1][pointer[1]] > 0) ++counter;
				if (layout[pointer[0]][pointer[1] - 1] > 0) ++counter;
				if (layout[pointer[0] - 1][pointer[1]] > 0) ++counter;
				if (counter < 2)
				{
					layout[pointer[0]][pointer[1]] = 2;
					generatedRooms.push_back(pointer);
					directionFound = true;
				}
				else 
				{
					movePointer(pointer, directions, 3 - j);
				}
			}
			else
			{
				movePointer(pointer, directions, 3 - j);
			}
		} 

		directionFound = false;

	}

	layout[pointer[0]][pointer[1]] = 4;
	generatedRooms.pop_back();

	for (int i = 0; i < 4 + 1; ++i)
	{
		std::random_shuffle(generatedRooms.begin(), generatedRooms.end());

		for (int j = 0; i < generatedRooms.size() && directionFound == false; ++j)
		{
			pointer = generatedRooms[j];

			std::random_shuffle(directions.begin(), directions.end());

			for (int k = 0; k < directions.size() && directionFound == false; ++k)
			{
				movePointer(pointer, directions, k);

				if (validatePointerValue(pointer[0]) && validatePointerValue(pointer[1]))
				{
					if (layout[pointer[0]][pointer[1]] == 0)
					{
						int counter = 0;
						if (validatePointerValue(pointer[1] + 1)) if (layout[pointer[0]][pointer[1] + 1] > 0) ++counter;
						if (validatePointerValue(pointer[0] + 1)) if (layout[pointer[0] + 1][pointer[1]] > 0) ++counter;
						if (validatePointerValue(pointer[1] - 1)) if (layout[pointer[0]][pointer[1] - 1] > 0) ++counter;
						if (validatePointerValue(pointer[0] - 1)) if (layout[pointer[0] - 1][pointer[1]] > 0) ++counter;
						if (counter < 2)
						{
							layout[pointer[0]][pointer[1]] = 2;
							generatedRooms.push_back(pointer);
							directionFound = true;
						}
						else
						{
							movePointer(pointer, directions, 3 - k);
						}
					}
					else
					{
						movePointer(pointer, directions, 3 - k);
					}
				}
				else 
				{
					movePointer(pointer, directions, 3 - k);
				}
			}

		}

		directionFound = false;
	}

	layout[pointer[0]][pointer[1]] = 3;

	for (int i = 0; i < layout.size(); i++) {
		for (int j = 0; j < layout[i].size(); j++)
			std::cout << layout[i][j] << " ";
		std::cout << std::endl;
	}
}