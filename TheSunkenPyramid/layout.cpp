#include <vector>
#include <iostream>

int main()
{
	std::vector<std::vector<int>>layout(11,{0,0,0,0,0,0,0,0,0,0,0});
	layout[5][5] = 1;
	std::vector<int>pointer(2, 5);
	std::vector<int>directions{ 1,2,3,4 };
	std::vector<std::vector<int>>generatedRooms;

	for (int i = 0; i < layout.size(); i++) {
		for (int j = 0; j < layout[i].size(); j++)
			std::cout << layout[i][j] << " ";
		std::cout << std::endl;
	}

	/*for (int i = 0; i < 4; ++i)
	{

	}*/
}