#include "Game/GameConfig.hpp"

#include <iostream>
#include <sstream>

#define WINDOW_DEFAULT_RES_X    ("WINDOW_DEFAULT_RES_X")
#define WINDOW_DEFAULT_RES_Y    ("WINDOW_DEFAULT_RES_Y")
#define WINDOW_TITLE			("WINDOW_TITLE")
#define IMAGE                   ("WINDOW_X_IMAGE")
#define SHADER                  ("WINDOW_X_SHADER")

//*******************************************************************
GameConfig::GameConfig()
{
	m_config = Config("../Code/Game/GameConfig.config");
}

//*******************************************************************
GameConfig::~GameConfig()
{
}

//*******************************************************************
void GameConfig::SetWindowResolutionX(int windowResolutionX)
{
	m_config.ConfigSet(WINDOW_DEFAULT_RES_X, windowResolutionX);
}

//*******************************************************************
void GameConfig::SetWindowResolutionY(int windowResolutionY)
{
	m_config.ConfigSet(WINDOW_DEFAULT_RES_Y, windowResolutionY);
}

//*******************************************************************
void GameConfig::SetWindowTitle(std::string windowTitle)
{
	m_config.ConfigSet(WINDOW_TITLE, windowTitle);
}

//*******************************************************************
bool GameConfig::GetWindowResolutionX(int &outValue)
{
	bool ableToGetOutValue = m_config.ConfigGetInt(outValue, WINDOW_DEFAULT_RES_X);
	return ableToGetOutValue;
}

//*******************************************************************
bool GameConfig::GetWindowResolutionY(int &outValue)
{
	bool ableToGetOutValue = m_config.ConfigGetInt(outValue, WINDOW_DEFAULT_RES_Y);
	return ableToGetOutValue;
}

//*******************************************************************
bool GameConfig::GetWindowTitle(std::string &outValue)
{
	bool ableToGetOutValue = m_config.ConfigGetString(outValue, WINDOW_TITLE);
	return ableToGetOutValue;
}

//*******************************************************************
bool GameConfig::GetImages(std::vector<std::string> &images)
{
    std::string outValue;
    bool ableToGetOutValue = m_config.ConfigGetString(outValue, IMAGE);

    if (ableToGetOutValue)
    {
        std::istringstream iStringStream(outValue);
        std::string stringToken;
        std::string partOfTheValue;
        while (std::getline(iStringStream, stringToken, '&'))
        {
            partOfTheValue = stringToken;
            images.push_back(partOfTheValue);
        }
    }

    return ableToGetOutValue;
}

//*******************************************************************
bool GameConfig::GetShaders(std::vector<std::string> &shaders)
{
    std::string outValue;
    bool ableToGetOutValue = m_config.ConfigGetString(outValue, SHADER);

    if (ableToGetOutValue)
    {
        std::istringstream iStringStream(outValue);
        std::string stringToken;
        std::string partOfTheValue;
        while (std::getline(iStringStream, stringToken, '&'))
        {
            partOfTheValue = stringToken;
            shaders.push_back(partOfTheValue);
        }
    }

    return ableToGetOutValue;
}

//*******************************************************************
void GameConfig::ListMembers()
{
	m_config.ConfigList();
}

