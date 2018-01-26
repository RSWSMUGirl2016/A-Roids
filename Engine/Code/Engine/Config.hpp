#pragma once
#include <map>
#include <vector>
#include <string>

#pragma once

class Config
{
public:	
	Config();
	Config(char const *configFile);
	~Config();

	bool ConfigSystemStartup(char const *configFile);
	void ConfigSystemShutdown();
    void AddMuliValueKeyToKeyValuePairs(std::vector<std::string> &keyValuePairs, std::string key, std::string value);
    void ConfigSet(std::string name, std::string value);
	void ConfigSet(std::string name, int const value);
	void ConfigSet(std::string name, float const value);
	void ConfigSet(std::string name, bool const value);
	bool ConfigGetString(std::string &outValue, std::string id);
	bool ConfigGetBool(bool &outValue, std::string id);
	bool ConfigGetInt(int &outValue, std::string id);
	bool ConfigGetFloat(float &outValue, std::string id);
	void ConfigList();

private:
	bool ConfigLoadFile(char const *fileName);
	bool IsStringAnInteger(std::string id);
	bool IsStringAFloat(std::string id);
	bool IsAQuotedString(std::string testString);
	std::string UnQuoteString(std::string testString);
	std::string AddQuotes(std::string testString);

private:
	char const *m_configFile;
	std::map<std::string, std::string> m_configMembers;
	std::vector<char> m_fileLines;
	std::vector<std::string> m_quotedKeys;
};
