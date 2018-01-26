#include "Engine/Config.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <typeinfo>

//*******************************************************************
Config::Config()
{
}

//*******************************************************************
Config::Config(char const *configFile)
{
	ConfigSystemStartup(configFile);
}

//*******************************************************************
Config::~Config()
{

}

//*******************************************************************
bool Config::ConfigSystemStartup(char const *configFile)
{
	m_configFile = configFile;

	std::ifstream inputFileStream;
	inputFileStream.open(configFile);

	bool wasFileFound = ConfigLoadFile(configFile);
	if (wasFileFound == false)
	{
		return false;
	}

	int indexFileLine = 0;
	while (!inputFileStream.eof())
	{
		std::string line;
		std::getline(inputFileStream, line);
		if (!inputFileStream)
		{
			return false;
		}

		std::size_t foundPoundSign = line.find("#");
		bool thereISAPoundSign = foundPoundSign < 200;
		if (thereISAPoundSign && foundPoundSign == 0)
		{
			m_fileLines.push_back('#');
			indexFileLine++;
			continue;
		}
		else if (thereISAPoundSign)
		{
			m_fileLines.push_back('f');
			indexFileLine++;
		}
		else
		{
			m_fileLines.push_back('p');
			indexFileLine++;
		}

		std::istringstream iStringStream(line);

		std::string stringToken;
		int indexInStringStream = 0;
		std::string name;
		std::string value;
		while (std::getline(iStringStream, stringToken, '='))
		{			
			foundPoundSign = stringToken.find("#");
			thereISAPoundSign = foundPoundSign < 200;

			if (indexInStringStream == 0)
			{
				if (thereISAPoundSign)
				{
					break;
				}
				name = stringToken;
				if (IsAQuotedString(name))
				{
					name = UnQuoteString(name);
					m_quotedKeys.push_back(name);
				}
			}
			else
			{
				if (thereISAPoundSign) 
				{
					int indexInStringStream2 = 0;
					std::istringstream iStringStreamStringToken(stringToken);
					while (std::getline(iStringStreamStringToken, stringToken, '#'))
					{
						if (indexInStringStream2 == 0)
						{
							value = stringToken;
						}
						indexInStringStream2++;
					}
				}	
				else
				{
					value = stringToken;
				}				
			}
			indexInStringStream++;
		}

        bool isThereAnExistingKey = m_configMembers.find(name) == m_configMembers.end();
        if (!isThereAnExistingKey)
        {
            std::string currentValue = m_configMembers.find(name)->second;
            m_configMembers[name] = currentValue + "&" + value;
        }
        else
        {
            m_configMembers[name] = value;
        }        
	}
	inputFileStream.close();
	return true;

}

//*******************************************************************
void Config::ConfigSystemShutdown()
{
	std::vector<std::string> keyValuePairs;
	typedef std::map<std::string, std::string>::iterator configIterator;
	for (configIterator iterator = m_configMembers.begin(); iterator != m_configMembers.end(); iterator++)
	{
		std::string key = iterator->first;
		std::string value = iterator->second;

		std::vector<std::string>::iterator it;

		it = find(m_quotedKeys.begin(), m_quotedKeys.end(), key);
		if (it != m_quotedKeys.end())
		{
			key = AddQuotes(key);
		}

        //Check if value has &std::string line;
        std::size_t foundAmpersand = value.find("&");
        bool thereISAnAmpersand = foundAmpersand < 200;
        if (thereISAnAmpersand)
        {
            AddMuliValueKeyToKeyValuePairs(keyValuePairs, key, value);
        }
        else
        {
            std::string keyValue = key + "=" + value;
            keyValuePairs.push_back(keyValue);
        }		
	}

	std::vector<std::string> outputFileLines;

	std::ifstream inputFileStream;
	inputFileStream.open(m_configFile);
	int fileLineCounter = 0;
    std::string line;
    int currentKeyValuePair = 0;
	while (std::getline(inputFileStream, line))
	{
		if (m_fileLines[fileLineCounter] == '#')
		{
			outputFileLines.push_back(line);
		} 
		else if (m_fileLines[fileLineCounter] == 'f')
		{
			std::string stringToken;
			std::istringstream iStringStreamStringToken(line);
			std::string commentString;
			int indexInStringStream = 0;
			while (std::getline(iStringStreamStringToken, stringToken, '#'))
			{
				if (indexInStringStream == 1)
				{
					commentString = stringToken;
				}
				indexInStringStream++;
			}
			std::string output = keyValuePairs[currentKeyValuePair] + "#" + commentString;
			outputFileLines.push_back(output);
            currentKeyValuePair++;
		}
		else
		{
			std::string output = keyValuePairs[currentKeyValuePair];
			outputFileLines.push_back(output);
            currentKeyValuePair++;
		}
		fileLineCounter++;
	}
	inputFileStream.close();
	

	std::ofstream configFile;
	configFile.open(m_configFile, std::ios::trunc);

	for (int outputLineCounter = 0; outputLineCounter < (int)outputFileLines.size(); outputLineCounter++)
	{
		configFile << outputFileLines[outputLineCounter] << "\n";
	}
	
	configFile.close();
}

//*******************************************************************
void Config::AddMuliValueKeyToKeyValuePairs(std::vector<std::string> &keyValuePairs, std::string key, std::string value)
{
    std::istringstream iStringStream(value);
    std::string stringToken;
    std::string partOfTheValue;
    while (std::getline(iStringStream, stringToken, '&'))
    {
        partOfTheValue = stringToken;
        if (IsAQuotedString(partOfTheValue))
        {
            partOfTheValue = AddQuotes(partOfTheValue);            
        }
        std::string keyValue = key + "=" + partOfTheValue;
        keyValuePairs.push_back(keyValue);
    }
}

//*******************************************************************
void Config::ConfigSet(std::string name, std::string value)
{
	std::string currentValue = m_configMembers.find(name)->second;
	if (IsAQuotedString(currentValue))
	{
		m_configMembers[name] = AddQuotes(value);
		return;
	}
	m_configMembers[name] = value;
}

//*******************************************************************
void Config::ConfigSet(std::string name, int const value)
{
	std::ostringstream oStringStream;
	oStringStream << value;
	std::string stringValue(oStringStream.str());
	m_configMembers[name] = stringValue;
}

//*******************************************************************
void Config::ConfigSet(std::string name, float const value)
{
	std::ostringstream oStringStream;
	oStringStream << value;
	std::string stringValue(oStringStream.str());

	std::string currentValue = m_configMembers.find(name)->second;
	if (IsAQuotedString(currentValue))
	{
		m_configMembers[name] = AddQuotes(stringValue);
		return;
	}
	m_configMembers[name] = stringValue;
}

//*******************************************************************
void Config::ConfigSet(std::string name, bool const value)
{
	std::string currentValue = m_configMembers.find(name)->second;
	
	if (value)
	{
		std::string valueTrue = "true";
		if (IsAQuotedString(valueTrue))
		{
			m_configMembers[name] = AddQuotes(valueTrue);
			return;
		}
		m_configMembers[name] = valueTrue;
	}
	else
	{
		std::string valueFalse = "false";
		if (IsAQuotedString(valueFalse))
		{
			m_configMembers[name] = AddQuotes(valueFalse);
			return;
		}
		m_configMembers[name] = valueFalse;
	}
}

//*******************************************************************
bool Config::ConfigGetString(std::string &outValue, std::string id)
{
	if (m_configMembers.find(id) == m_configMembers.end())
	{
		//DebuggerPrintf("%s, key is not a part of the member of %s", id, m_configFile);
		return false;
	}

	std::string value = m_configMembers.find(id)->second;
	bool isQuoted = IsAQuotedString(value);
	if (isQuoted)
	{
		outValue = UnQuoteString(value);
		return true;
	}
	outValue = value;
	return true;
}

//*******************************************************************
bool Config::ConfigGetBool(bool &outValue, std::string id)
{
	if (m_configMembers.find(id) == m_configMembers.end())
	{
		DebuggerPrintf("%s, key is not a part of the member of %s", id, m_configFile);
		return false;
	}

	std::string value = m_configMembers.find(id)->second;
	std::string newValue;
	bool isQuoted = IsAQuotedString(value);
	if (isQuoted)
	{
		newValue = UnQuoteString(value);
	}
	newValue = value;

	if (newValue.compare("true") != 0 || newValue.compare("false") != 0) {
		DebuggerPrintf("Value of %s is not a boolean", id);
		return false;
	}

	if (newValue.compare("true") == 0)
	{
		bool valueTrue = true;
		outValue = valueTrue;
	}
	else
	{
		bool valueFalse = false;
		outValue = valueFalse;
	}
	return true;
}

//*******************************************************************
bool Config::ConfigGetInt(int &outValue, std::string id)
{
	if (m_configMembers.find(id) == m_configMembers.end())
	{
		DebuggerPrintf("%s, key is not a part of the member of %s", id, m_configFile);
		return false;
	}

	if (!IsStringAnInteger(id))
	{
		DebuggerPrintf("Value of %s is not an integer", id);
		return false;
	}
	
	std::string::size_type sizeT;
	std::string stringValue = m_configMembers.find(id)->second;
	std::string newValue;
	bool isQuoted = IsAQuotedString(stringValue);
	if (isQuoted)
	{
		newValue = UnQuoteString(stringValue);
	}
	else
	{
		newValue = stringValue;
	}	

	int value = std::stoi(newValue, &sizeT);
	outValue = value;
	return true;
}

//*******************************************************************
bool Config::ConfigGetFloat(float &outValue, std::string id)
{
	if (m_configMembers.find(id) == m_configMembers.end())
	{
		DebuggerPrintf("%s, key is not a part of the member of %s", id, m_configFile);
		return false;
	}

	if (!IsStringAFloat(id))
	{
		DebuggerPrintf("Value of %s is not a float", id);
		return false;
	}

	std::string::size_type sizeT;
	std::string stringValue = m_configMembers.find(id)->second;
	std::string newValue;
	bool isQuoted = IsAQuotedString(stringValue);
	if (isQuoted)
	{
		newValue = UnQuoteString(stringValue);
	}
	else
	{
		newValue = stringValue;
	}

	float value = std::stof(newValue, &sizeT);
	outValue = value;
	return true;
}

//*******************************************************************
void Config::ConfigList()
{
	DebuggerPrintf("%s config members\r\n", m_configFile);
	typedef std::map<std::string, std::string>::iterator configIterator;
	for (configIterator iterator = m_configMembers.begin(); iterator != m_configMembers.end(); iterator++) 
	{
		std::string key = iterator->first;
		char const * cKey = key.c_str();
		std::string value = iterator->second;
		char const * cValue = value.c_str();
		DebuggerPrintf("m_configMembers[%s]=%s\r\n", cKey, cValue);
	}
}

//*******************************************************************
bool Config::ConfigLoadFile(char const *fileName)
{
	std::ifstream inputFileStream;
	inputFileStream.open(fileName);
	if (!inputFileStream.is_open())
	{
		DebuggerPrintf("Cannot open file");
		return false;
	}
	inputFileStream.close();
	return true;
}

//*******************************************************************
bool Config::IsStringAnInteger(std::string id)
{
	std::string value = m_configMembers.find(id)->second;
	std::string newValue;
	bool isQuoted = IsAQuotedString(value);
	if (isQuoted)
	{
		newValue = UnQuoteString(value);
	}
	newValue = value;

	std::string::const_iterator stringIteratior = newValue.begin();
	bool isValueAnInteger = false;
	int stringCounter = 0;

	while (stringIteratior != newValue.end() && isdigit(*stringIteratior))
	{
		if (!isdigit(*stringIteratior))
		{
			break;
		}
		++stringCounter;
		++stringIteratior;
	}

	int valueLength = (int)newValue.length();
	if (stringCounter == valueLength)
	{
		isValueAnInteger = true;
	}

	return isValueAnInteger;
}

//*******************************************************************
bool Config::IsStringAFloat(std::string id)
{
	std::string value = m_configMembers.find(id)->second;
	std::string newValue;
	bool isQuoted = IsAQuotedString(value);
	if (isQuoted)
	{
		newValue = UnQuoteString(value);
	}
	newValue = value;

	std::string::const_iterator stringIteratior = newValue.begin();
	bool isValueANumber = false;
	bool isValueAFloat = false;
	bool isThereADotInTheValue = false;
	int stringCounter = 0;

	while (stringIteratior != newValue.end() && isdigit(*stringIteratior))
	{
		if (!isdigit(*stringIteratior))
		{
			if (*stringIteratior != '.')
			{
				break;
			}
			else
			{
				isThereADotInTheValue = true;
			}
		}
		++stringCounter;
		++stringIteratior;
	}

	int valueLength = (int)newValue.length();
	if (stringCounter == valueLength)
	{
		isValueANumber = true;
		if (isThereADotInTheValue)
		{
			isValueAFloat = true;
		}
	}
	
	return isValueAFloat;
}

//*******************************************************************
bool Config::IsAQuotedString(std::string testString)
{
	if (testString.at(0) == '"')
	{
		return true;
	}
	return false;
}

//*******************************************************************
std::string Config::UnQuoteString(std::string testString)
{
	std::string newString = testString.substr(1, testString.size() - 2);
	return newString;
}

std::string Config::AddQuotes(std::string testString)
{
	std::string newString = '"' + testString + '"';
	return newString;
}
