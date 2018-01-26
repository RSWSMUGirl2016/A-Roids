#pragma once

#if !defined( __TOOLS_FBXLOAD__ )
#define __TOOLS_FBXLOAD__

#include "Engine/MeshBuilder/MeshBuilder.hpp"
#include "Engine/MeshBuilder/Skeleton.hpp"
#include "Engine/MeshBuilder/Motion.hpp"
#include "Engine/MeshBuilder/SkeletonInstance.hpp"
#include "Engine/MeshBuilder/FileBinaryStream.hpp"

const unsigned int SKELETON = 0;
const unsigned int MESH = 1;
const unsigned int MOTION = 2;
const unsigned int IMPORT_ALL = 3;

void FbxListFile(char const *filename);
bool FbxLoadMesh(MeshBuilder *mb, char const *filename);
bool FbxLoadSkeleton(Skeleton *skel, char const *filename);
bool FbxLoadMotionSkeleton(Motion *motion, Skeleton const *skeleton, char const *filename, unsigned int framerate = 10);
bool FbxLoadMesh(MeshBuilder *mb, char const *filename, Skeleton const *skeleton);
bool FbxLoadMotion(Motion *motion, Skeleton const *skeleton, char const *filename, unsigned int framerate = 10);
unsigned int ConvertFBXFileToEngineFiles(char const *fbx_filename, char const *root_output_name, float impor_scale = 1.0f, unsigned int import_flags = IMPORT_ALL);
MeshBuilder* ConvertEngineMeshFileToFBXFile(char const *filename);
Skeleton* ConvertEngineSkeletonFileToFBXFile(char const *filename);
Motion* ConvertEngineMotionFileToFBXFile(char const *filename);

#elif



#endif
