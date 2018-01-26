#pragma once
#include "Engine/Config.hpp"

struct glyph_info_t
{
    int m_id;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_xOffset;
    int m_yOffset;
    int m_xAdvance;

};

struct kerning_t {
    int m_first;
    int m_second;
    int m_offset;
};

class Font
{
public:
    Font();
    ~Font();

    void LoadFromFile(std::string fileName);

    glyph_info_t GetGlyph(char const character);
    
    glyph_info_t GetInvalidGlyph();
    int GetKerningOffset(char const prevCharacter, char const character);
    char const *m_name;
    int m_textureCount;
    //char const **m_textureFilepaths;
    std::string m_textureFilepath;
    int  m_size;
    int  m_lineHeight;
    int  m_scaleW;
    int  m_scaleH;
    int  m_pages;
    bool m_bold;
    bool m_italic;

    std::map<int, glyph_info_t> m_glyphInfos;
    std::map<std::pair<int, int>, int> m_kernings;
};