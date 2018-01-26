/////////////////////////////////////////////////////////////////////////////////////////////////
// Credit to Squirrel Eiserloh
// FileUtilities.hpp
//
// Basic file i/o utility functions.
//
#pragma once
#include <string>
#include <vector>


//-----------------------------------------------------------------------------------------------
bool LoadBinaryFileToBuffer(const std::string& filePath, std::vector< unsigned char >& out_buffer);
bool SaveBinaryFileFromBuffer(const std::string& filePath, const std::vector< unsigned char >& buffer);

//*******************************************************************
void PushFloatToBuffer(float f, std::vector<unsigned char>& buffer);
float GetFloatFromBuffer(int floatIndex, std::vector<unsigned char>& buffer);