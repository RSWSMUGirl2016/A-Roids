#include "Engine/Core/XMLUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <typeinfo>

//*******************************************************************
int ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, int defaultValue)
{
    int returnValue = defaultValue;
    element.FindAttribute(attributeName)->QueryIntValue(&returnValue);
    return returnValue;
}

//*******************************************************************
char ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, char defaultValue)
{
    char returnValue = defaultValue;
    char const *charValue = element.Attribute(attributeName);

    ASSERT_OR_DIE(charValue != nullptr, "XMLUtils: Char attribute does not exist");
    returnValue = charValue[0];

    return returnValue;
}

//*******************************************************************
bool ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, bool defaultValue)
{
    bool returnValue = defaultValue;
    element.FindAttribute(attributeName)->QueryBoolValue(&returnValue);
    return returnValue;
}

//*******************************************************************
float ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, float defaultValue)
{
    float returnValue = defaultValue;
    element.FindAttribute(attributeName)->QueryFloatValue(&returnValue);
    return returnValue;
}

//*******************************************************************
Rgba ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, const Rgba& defaultValue)
{
    Rgba returnValue = defaultValue;
    char const *colorValue = element.Attribute(attributeName);

    ASSERT_OR_DIE(colorValue != nullptr, "XMLUtils: Rgba attribute does not exist");

    std::istringstream iStringStream(colorValue);
    std::string stringToken;

    int currentIndex = 0;

    while (std::getline(iStringStream, stringToken, ','))
    {
        std::string::size_type size;
        int color = std::stoi(stringToken, &size);

        if (currentIndex == 0)
            returnValue.r = (unsigned char)color;
        if (currentIndex == 1)
            returnValue.g = (unsigned char)color;
        if (currentIndex == 2)
            returnValue.b = (unsigned char)color;
        if (currentIndex == 3)
            returnValue.a = (unsigned char)color;

        currentIndex++;
    }

    ASSERT_OR_DIE(currentIndex == 4, "XMLUtils: Rgba attribute is not the correct number of integers to constitute as a RGBA.");

    return returnValue;
}

//*******************************************************************
Vector2 ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, const Vector2& defaultValue)
{
    Vector2 returnValue = defaultValue;
    char const *vectorTwoValue = element.Attribute(attributeName);
    
    ASSERT_OR_DIE(vectorTwoValue != nullptr, "XMLUtils: Vector2 attribute does not exist");

    std::istringstream iStringStream(vectorTwoValue);
    std::string stringToken;

    int currentIndex = 0;

    while (std::getline(iStringStream, stringToken, ','))
    {
        std::string::size_type size;
        float vectorToken = std::stof(stringToken, &size);

        if (currentIndex == 0)
            returnValue.x = vectorToken;
        if (currentIndex == 1)
            returnValue.y = vectorToken;

        currentIndex++;
    }

    ASSERT_OR_DIE(currentIndex == 2, "XMLUtils: Vector2 attribute is not the correct number of integers to constitute as a Vector2.");

    return returnValue;
}

//*******************************************************************
Vector3 ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, const Vector3& defaultValue)
{
    Vector3 returnValue = defaultValue;
    char const *vectorTwoValue = element.Attribute(attributeName);

    ASSERT_OR_DIE(vectorTwoValue != nullptr, "XMLUtils: Vector3 attribute does not exist");

    std::istringstream iStringStream(vectorTwoValue);
    std::string stringToken;

    int currentIndex = 0;

    while (std::getline(iStringStream, stringToken, ','))
    {
        std::string::size_type size;
        float vectorToken = std::stof(stringToken, &size);

        if (currentIndex == 0)
            returnValue.x = vectorToken;
        if (currentIndex == 1)
            returnValue.y = vectorToken;
        if (currentIndex == 2)
            returnValue.z = vectorToken;

        currentIndex++;
    }

    ASSERT_OR_DIE(currentIndex == 3, "XMLUtils: Vector3 attribute is not the correct number of floats to constitute as a Vector3.");

    return returnValue;
}

//*******************************************************************
IntVector2 ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, const IntVector2& defaultValue)
{
    IntVector2 returnValue = defaultValue;
    char const *vectorTwoValue = element.Attribute(attributeName);

    ASSERT_OR_DIE(vectorTwoValue != nullptr, "XMLUtils: IntVector2 attribute does not exist");

    std::istringstream iStringStream(vectorTwoValue);
    std::string stringToken;

    int currentIndex = 0;

    while (std::getline(iStringStream, stringToken, ','))
    {
        std::string::size_type size;
        int vectorToken = std::stoi(stringToken, &size);

        if (currentIndex == 0)
            returnValue.x = vectorToken;
        if (currentIndex == 1)
            returnValue.y = vectorToken;

        currentIndex++;
    }

    ASSERT_OR_DIE(currentIndex == 2, "XMLUtils: Vector2 attribute is not the correct number of integers to constitute as a IntVector2.");

    return returnValue;
}

//*******************************************************************
std::string ParseXmlAttribute(const tinyxml2::XMLElement& element, const char* attributeName, const std::string& defaultValue)
{
    std::string returnValue = defaultValue;

    char const *stringValue = element.Attribute(attributeName);

    ASSERT_OR_DIE(stringValue != nullptr, "XMLUtils: String attribute does not exist");

    returnValue = std::string(stringValue);
    return returnValue;
}

//*******************************************************************
void ValidationXmlElement(const tinyxml2::XMLElement& element, const char* commaSeparatedListOfValidChildElementNames, const char* commaSeparatedListOfValidAttributeNames)
{
    //char const *elementAttribute = element.FindAttribute(attributeName);
    //ASSERT_OR_DIE(vectorTwoValue == nullptr, "XMLUtils: IntVector2 attribute with name: %s, does not exist", attributeName);

    std::istringstream iStringChildStream(commaSeparatedListOfValidChildElementNames);
    std::string stringToken;

    while (std::getline(iStringChildStream, stringToken, ','))
    {
        const tinyxml2::XMLElement* childElement = element.FirstChildElement(stringToken.c_str());
        ASSERT_OR_DIE(childElement != nullptr, "XMLUtils, Validation: Child element does not exist");
    }

    std::istringstream iStringAttributeStream(commaSeparatedListOfValidAttributeNames);

    while (std::getline(iStringAttributeStream, stringToken, ','))
    {
        const char* attributeName = element.Attribute(stringToken.c_str());
        ASSERT_OR_DIE(attributeName != nullptr, "XMLUtils, Validation: Attribute does not exist");
    }
}
