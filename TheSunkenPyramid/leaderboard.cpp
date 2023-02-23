#include "sqlite3.h"
#include "leaderboard.h"
#include "SFML/Graphics.hpp"
#include "constants.h"
#include <assert.h>
#include <windows.h>

int gNumRecords;

void DebugPrint(const std::string& mssg1, const std::string& mssg2)
{
	OutputDebugString(mssg1.c_str());
	OutputDebugString("\n");
	if (!mssg2.empty()) {
		OutputDebugString(mssg2.c_str());
		OutputDebugString("\n");
	}
}

int callbackRecords(void* p, int numF, char** arv, char** arn)
{
	gNumRecords++;
	return 0;
}

int sqlite3_prepare(
	sqlite3* db,            /* Database handle */
	const char* zSql,       /* SQL statement, UTF-8 encoded */
	int nByte,              /* Maximum length of zSql in bytes. */
	sqlite3_stmt** ppStmt,  /* OUT: Statement handle */
	const char** pzTail     /* OUT: Pointer to unused portion of zSql */
);

void Leaderboard::init()
{
	gNumRecords = 0;
	char* zErrMsg = nullptr;
	int rc = sqlite3_open("data/scores.db", &db);
}

void Leaderboard::create()
{
	//Create db file
	sqlite3* db;
	int rc = sqlite3_open("data/scores.db", &db);

	//Enable foreign key constraints
	const std::string sql4 = "PRAGMA foreign_keys = ON;";  \
		rc = sqlite3_exec(db, sql4.c_str(), nullptr, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		DebugPrint("SQL error: ", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		DebugPrint("pragma yay", "");
	}

	//Create tables
	gNumRecords = 0;
	const std::string sql1 = "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'Leaderboard'";
	rc = sqlite3_exec(db, sql1.c_str(), callbackRecords, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		sqlite3_free(zErrMsg);
	}
	else if (gNumRecords == 0) {
		char* zErrMsg = nullptr;

		const std::string sql2 = "CREATE TABLE Player("  \
			"PlayerID INTEGER PRIMARY KEY     NOT NULL," \
			"Name                  STRING     NOT NULL);"; \
			rc = sqlite3_exec(db, sql2.c_str(), nullptr, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			DebugPrint("SQL error: ", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			DebugPrint("Table created successfully","");
		}


		const std::string sql3 = "CREATE TABLE Leaderboard("  \
			"ScoreID  INTEGER PRIMARY KEY     NOT NULL," \
			"Score                INTEGER     NOT NULL," \
			"PlayerID             INTEGER     NOT NULL," \
			"FOREIGN KEY (PlayerID) REFERENCES Player(PlayerID));"; \
		rc = sqlite3_exec(db, sql3.c_str(), nullptr, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			DebugPrint("SQL error: ", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			DebugPrint("Table created successfully","");
		}
	}
}

void Leaderboard::insertPlayer(std::string& values)
{
	//string 1 format "(num, 'name');"
	const std::string sql1 = "INSERT INTO Player (PlayerID, Name) "\
		"VALUES " + values;
	rc = sqlite3_exec(db, sql1.c_str(), nullptr, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		DebugPrint("SQL error: ", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		DebugPrint("Data inserted successfully", "");
	}
}
void Leaderboard::insertLeaderboard(std::string& values)
{
	//string 2 format "(num, num, num);"
	const std::string sql2 = "INSERT INTO Leaderboard (ScoreID, Score, PlayerID) "\
		"VALUES " + values;
	rc = sqlite3_exec(db, sql2.c_str(), nullptr, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		DebugPrint("SQL error: ", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		DebugPrint("Data inserted successfully", "");
	}
}

void Leaderboard::remove(int ScoreID)
{
	bool deletePlayer = false;

	sqlite3_stmt* stmt;
	const std::string sql1 = "SELECT PlayerID FROM Leaderboard WHERE ScoreID = '" + std::to_string(ScoreID) + "'";
	sqlite3_prepare(db, sql1.c_str(), -1, &stmt, 0);
	sqlite3_step(stmt);
	const int PlayerID = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	const std::string sql2 = "DELETE FROM Leaderboard WHERE ScoreID = '" + std::to_string(ScoreID) + "'";
	rc = sqlite3_exec(db, sql2.c_str(), nullptr, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		DebugPrint("SQL error: ", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		DebugPrint("Data deleted successfully", "");
	}

	const std::string sql3 = "SELECT PlayerID FROM Leaderboard WHERE PlayerID = '" + std::to_string(PlayerID) + "'";
	sqlite3_prepare(db, sql3.c_str(), -1, &stmt, 0);
	sqlite3_step(stmt);
	if (sqlite3_column_int(stmt, 0) == 0)
	{
		deletePlayer = true;
	}
	sqlite3_finalize(stmt);

	if (deletePlayer) 
	{
		const std::string sql2 = "DELETE FROM Player WHERE PlayerID = '" + std::to_string(PlayerID) + "'";
		rc = sqlite3_exec(db, sql2.c_str(), nullptr, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			DebugPrint("SQL error: ", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			DebugPrint("Data deleted successfully", "");
		}
	}
}

std::vector<std::vector<int>> Leaderboard::retrieveScores()
{
	sqlite3_stmt* stmt;

	const std::string sql1 = "SELECT COUNT(*) FROM Leaderboard";
	sqlite3_prepare(db, sql1.c_str(), -1, &stmt, 0);
	sqlite3_step(stmt);
	const int Length = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);

	std::vector<std::vector<int>>sortedScores;
	std::vector<int> toBeSorted = { 2, 0 };

	const std::string sql2 = "SELECT ScoreID FROM Leaderboard";
	sqlite3_prepare(db, sql2.c_str(), -1, &stmt, 0);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		sortedScores.push_back({ sqlite3_column_int(stmt, 0) });
	}
	sqlite3_finalize(stmt);

	const std::string sql3 = "SELECT Score FROM Leaderboard";
	sqlite3_prepare(db, sql3.c_str(), -1, &stmt, 0);
	int index = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		sortedScores[index].push_back({sqlite3_column_int(stmt, 0)});
		++index;
	}
	sqlite3_finalize(stmt);

	const std::string sql4 = "SELECT PlayerID FROM Leaderboard";
	sqlite3_prepare(db, sql4.c_str(), -1, &stmt, 0);
	index = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		sortedScores[index].push_back({ sqlite3_column_int(stmt, 0) });
		++index;
	}
	sqlite3_finalize(stmt);

	const std::string sql5 = "SELECT Name FROM Player";
	sqlite3_prepare(db, sql5.c_str(), -1, &stmt, 0);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		const unsigned char* temp = sqlite3_column_text(stmt, 0);
		std::string strtemp(reinterpret_cast<const char*>(temp));
		PlayerName.push_back(strtemp);
	}
	sqlite3_finalize(stmt);


	bool swapMade = true;
	std::vector<std::vector<int>>tempvec{{0}};
	while (swapMade)
	{
		swapMade = false;
		for (int i = 0; i < sortedScores.size() - 1; ++i)
		{
			if (sortedScores[i][1] > sortedScores[i + 1][1])
			{
				tempvec[0] = sortedScores[i];
				sortedScores[i] = sortedScores[i + 1];
				sortedScores[i + 1] = tempvec[0];
				swapMade = true;
			}
		}
	}
	return sortedScores;
}
