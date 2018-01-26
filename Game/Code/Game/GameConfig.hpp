#pragma once
#include "Engine/Config.hpp"

class GameConfig
{
public:
	GameConfig();
	~GameConfig();

	void SetWindowResolutionX(int windowResolutionX);
	void SetWindowResolutionY(int windowResolutionY);
	void SetWindowTitle(std::string windowTitle);
	bool GetWindowResolutionX(int &outValue);
	bool GetWindowResolutionY(int &outValue);
	bool GetWindowTitle(std::string &outValue);
    bool GetImages(std::vector<std::string> &images);
    bool GetShaders(std::vector<std::string> &shaders);
	void ListMembers();

	Config		m_config;
};