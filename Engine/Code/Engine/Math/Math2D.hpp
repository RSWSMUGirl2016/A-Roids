#pragma once
#include "Engine/Math/AABB2D.hpp"
#include "Engine/Math/Disc2D.hpp"

bool DoAABB2DsOverlap(const AABB2D& first, const AABB2D& second);
bool DoDisc2DsOverlap(const Disc2D& first, const Disc2D& second);

void BounceDiscs(Disc2D& first, Disc2D& second, Vector2& firstVelocity, Vector2& secondVelocity);
void BounceAABB2DAndDisc2D(AABB2D& aabb2d, Disc2D& disc2d);

