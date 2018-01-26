#include "Engine/Font/Font.hpp"
#include "ThirdParty/tinyXML/tinyxml2-master/tinyxml2.h"
#include "Engine/Core/ErrorWarningAssert.hpp"

using namespace tinyxml2;

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { DebuggerPrintf("Error: %i\n", a_eResult); return a_eResult; }
#endif

//*******************************************************************
Font::Font()
    : m_name(nullptr)
    , m_textureCount(0)
    , m_textureFilepath("test")
    , m_size(0)
    , m_lineHeight(0)
    , m_scaleW(0)
    , m_scaleH(0)
    , m_pages(0)
    , m_bold(false)
    , m_italic(false)
{
}

//*******************************************************************
Font::~Font()
{
}

//*******************************************************************
void Font::LoadFromFile(std::string fileName)
{
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile(fileName.c_str());
    ASSERT_OR_DIE(eResult == XML_SUCCESS, "Error in loading file from file, could not find .fnt file\n");

    XMLNode *pRoot = xmlDoc.RootElement();
    ASSERT_OR_DIE(pRoot != nullptr, "Error in loading file from file, could not find root\n");

    XMLElement *pElement = pRoot->FirstChildElement("info");
    m_name = pElement->Attribute("face");
    m_size = pElement->IntAttribute("size");
    m_bold = pElement->BoolAttribute("bold");
    m_italic = pElement->BoolAttribute("italic");

    pElement = pRoot->FirstChildElement("common");
    m_lineHeight = pElement->IntAttribute("lineHeight");
    m_scaleW = pElement->IntAttribute("scaleW");
    m_scaleH = pElement->IntAttribute("scaleH");
    m_pages = pElement->IntAttribute("pages");

    pElement = pRoot->FirstChildElement("chars");
    ASSERT_OR_DIE(pElement != nullptr, "Error in loading file from file, could not find CHAR child element\n");

    for (auto xml_char = pElement->FirstChildElement("char"); xml_char != nullptr; xml_char = xml_char->NextSiblingElement())
    {
        glyph_info_t glyph;

        glyph.m_id = xml_char->IntAttribute("id");
        glyph.m_x = xml_char->IntAttribute("x");
        glyph.m_y = xml_char->IntAttribute("y");
        glyph.m_width = xml_char->IntAttribute("width");
        glyph.m_height = xml_char->IntAttribute("height");
        glyph.m_xOffset = xml_char->IntAttribute("xoffset");
        glyph.m_yOffset = xml_char->IntAttribute("yoffset");
        glyph.m_xAdvance = xml_char->IntAttribute("xadvance");

        m_glyphInfos[glyph.m_id] = glyph;
    }


    pElement = pRoot->FirstChildElement("kernings");
    ASSERT_OR_DIE(pElement != nullptr, "Error in loading file from file, could not find KERNINGS child element\n");
    for (auto xml_char = pElement->FirstChildElement("kerning"); xml_char != nullptr; xml_char = xml_char->NextSiblingElement())
    {
        kerning_t kerning;

        kerning.m_first = xml_char->IntAttribute("first");
        kerning.m_second = xml_char->IntAttribute("second");
        kerning.m_offset = xml_char->IntAttribute("amount");

        std::pair <int, int> kerningPair;
        kerningPair = std::make_pair(kerning.m_first, kerning.m_second);
        m_kernings[kerningPair] = kerning.m_offset;

    }

    pElement = pRoot->FirstChildElement("pages");
    ASSERT_OR_DIE(pElement != nullptr, "Error in loading file from file, could not find PAGES child element");

    for (auto xml_char = pElement->FirstChildElement("page"); xml_char != nullptr; xml_char = xml_char->NextSiblingElement())
    {
        m_textureCount = xml_char->IntAttribute("id");
        std::string file(xml_char->Attribute("file"));
        m_textureFilepath = "Data/Fonts/" + file;
    }
}

//*******************************************************************
glyph_info_t Font::GetGlyph(char const character)
{
    return m_glyphInfos[(int)character];
}

//*******************************************************************
glyph_info_t Font::GetInvalidGlyph()
{
    return m_glyphInfos[-1];
}

//*******************************************************************
int Font::GetKerningOffset(char const prevCharacter, char const character)
{
    std::pair <int, int> kerningPair;
    kerningPair = std::make_pair((int)prevCharacter, (int)character);
    return m_kernings[kerningPair];
}