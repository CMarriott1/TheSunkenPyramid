#pragma once

#include "SFML/Graphics.hpp"
#include "constants.h"
#include "sqlite3.h"

struct Leaderboard
{
	sqlite3* db;
	char* zErrMsg = nullptr;
	int rc;

	std::vector<std::string>PlayerName;

	void init();
	void create();
	int insertPlayer(std::string);
	void insertLeaderboard(std::string);
	void remove(int);
	std::vector<std::vector<int>> retrieveScores();
};