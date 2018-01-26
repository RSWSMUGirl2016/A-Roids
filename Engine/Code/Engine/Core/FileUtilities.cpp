#include "Engine/Core/FileUtilities.hpp"


//-----------------------------------------------------------------------------------------------
bool LoadBinaryFileToBuffer(const std::string& filePath, std::vector< unsigned char >& out_buffer)
{
	FILE* file = nullptr;
	errno_t errorCode = fopen_s(&file, filePath.c_str(), "rb"); // open for reading in binary mode
	if (errorCode)
		return false;

	// Determine file size: "Seek" to to end of file, "tell" where we're at, then back to start
	fseek(file, 0, SEEK_END);
	size_t numBytesToRead = ftell(file);
	rewind(file);

	out_buffer.resize(numBytesToRead);
	if (numBytesToRead > 0)
		fread(&out_buffer[0], sizeof(unsigned char), numBytesToRead, file);

	fclose(file);
    return true;
}


//-----------------------------------------------------------------------------------------------
bool SaveBinaryFileFromBuffer(const std::string& filePath, const std::vector< unsigned char >& buffer)
{
	FILE* file = nullptr;
	errno_t errorCode = fopen_s(&file, filePath.c_str(), "wb"); // open for writing in binary mode
	if (errorCode)
		return false;

	size_t numBytesToWrite = (int)buffer.size();
	if (numBytesToWrite > 0)
		fwrite(&buffer[0], sizeof(unsigned char), numBytesToWrite, file);

	fclose(file);
	return true;
}

//*******************************************************************
void PushFloatToBuffer(float f, std::vector<unsigned char>& buffer)
{
	unsigned char* ptrToFloatBytes = (unsigned char*)(&f);
	buffer.push_back(ptrToFloatBytes[0]);
	buffer.push_back(ptrToFloatBytes[1]);
	buffer.push_back(ptrToFloatBytes[2]);
	buffer.push_back(ptrToFloatBytes[3]);
}

//*******************************************************************
float GetFloatFromBuffer(int floatIndex, std::vector<unsigned char>& buffer)
{
	unsigned char* floatBytes = &buffer[floatIndex*4];
	float* fPtr = (float*)floatBytes;
	float f = *fPtr;
	return f;
}

