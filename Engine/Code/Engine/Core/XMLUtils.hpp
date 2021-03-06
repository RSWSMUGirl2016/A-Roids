#pragma once
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/IntVector2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "ThirdParty/tinyXML/tinyxml2-master/tinyxml2.h"

#include <string>

//*******************************************************************
int ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, int defaultValue);
char ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, char defaultValue);
bool ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, bool defaultValue);
float ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, float defaultValue);
Rgba ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, const Rgba& defaultValue);
Vector2 ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, const Vector2& defaultValue);
Vector3 ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, const Vector3& defaultValue);
IntVector2 ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, const IntVector2& defaultValue);
std::string ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, const std::string& defaultValue);

//*******************************************************************
void ValidationXmlElement(const tinyxml2::XMLElement& element, const char* commaSeparatedListOfValidChildElementNames, const char* commaSeparatedListOfValidAttributeNames);
