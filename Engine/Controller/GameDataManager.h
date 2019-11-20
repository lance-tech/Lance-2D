#pragma once

#include <queue>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class GameData
{
public:
	int Score;
	int Level;
	std::string Name;
};

class CompareData
{
public:
	virtual bool operator() (const GameData& source, const GameData& dest)
	{
		return source.Score < dest.Score;
	}
};

class GameDataManager
{
public:
	GameDataManager();
	~GameDataManager();

	static bool SaveGameData();
	static std::string GetTopScore();
	static void ResetScores();

	static void SetUserScore(int value);
	static void SetUserLevel(int value);
	static void SetUserName(std::string name);
	static GameData& GetUserData();

private:
	static void SetGameData();
	static GameData UserData;
	static std::priority_queue<GameData, std::vector<GameData>, CompareData> DataContainer;
};

