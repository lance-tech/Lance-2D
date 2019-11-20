#include "GameDataManager.h"

GameData GameDataManager::UserData;
std::priority_queue<GameData, std::vector<GameData>, CompareData> GameDataManager::DataContainer;


GameDataManager::GameDataManager()
{
}


GameDataManager::~GameDataManager()
{
}

std::string GameDataManager::GetTopScore()
{
	std::string score = "";
	if (DataContainer.size() > 0)
	{
		GameData data = DataContainer.top();
		score = std::to_string(data.Score);
		DataContainer.pop();
	}
	return score;
}

void GameDataManager::SetGameData()
{
	std::ifstream gameFile("Game Data.txt");
	if (gameFile.is_open())
	{
		int score, level;
		while (gameFile >> score >> level)
		{
			GameData data;
			data.Score = score;
			data.Level = level;

			DataContainer.push(data);
		}
	}
	else
	{
		//std::cout << "Game Data Read Error!";
	}
}

bool GameDataManager::SaveGameData()
{
	std::ofstream gameFile;
	gameFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);

	try
	{
		gameFile.open("Game Data.txt", std::ios_base::app);
		gameFile << UserData.Score << " " << UserData.Level << "\n";
		gameFile.close();

		SetGameData();

		return true;
	}
	catch (std::ofstream::failure e)
	{
		//std::cout << e.what();
		return false;
	}
}

void GameDataManager::ResetScores()
{
	while (!DataContainer.empty())
	{
		DataContainer.pop();
	}
}

void GameDataManager::SetUserScore(int value)
{
	UserData.Score = value;
}

void GameDataManager::SetUserLevel(int value)
{
	UserData.Level = value;
}

void GameDataManager::SetUserName(std::string name)
{
	UserData.Name = name;
}

GameData& GameDataManager::GetUserData()
{
	return UserData;
}