#pragma once
#include "Engine/Math/AABB3D.hpp"
#include "Engine/Math/Sphere3D.hpp"

bool DoAABB3DsOverlap(const AABB3D& first, const AABB3D& second);
bool DoDisc3DsOverlap(const Sphere3D& first, const Sphere3D& second);
