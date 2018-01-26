#pragma once
#include "Engine/Math/MathUtilities.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"

unsigned int Get1dNoiseUint(int positionX, unsigned int seed);
inline unsigned int Get2dNoiseUint(int indexX, int indexY, unsigned int seed);
inline unsigned int Get3dNoiseUint(int indexX, int indexY, int indexZ, unsigned int seed);
inline unsigned int Get4dNoiseUint(int indexX, int indexY, int indexZ, int indexT, unsigned int seed);
//-----------------------------------------------------------------------------------------------
// Credit to Squirrel Eiserloh
// Perlin noise functions (random-access / deterministic)
//
// Perlin noise is slightly more expensive, but more organic-looking (less axial) than regular
//	square fractal noise, through the use of blended dot products vs. randomized gradient vectors.
//
// <numOctaves>			Number of layers of noise added together
// <octavePersistence>	Amplitude multiplier for each subsequent octave (each octave is quieter)
// <octaveScale>		Frequency multiplier for each subsequent octave (each octave is busier)
// <renormalize>		If true, uses nonlinear (SmoothStep) renormalization to within [-1,1]
//
float Compute1dPerlinNoise(float position, float scale = 1.f, unsigned int numOctaves = 1, float octavePersistence = 0.5f, float octaveScale = 2.f, bool renormalize = true, unsigned int seed = 0);
float Compute2dPerlinNoise(float posX, float posY, float scale = 1.f, unsigned int numOctaves = 1, float octavePersistence = 0.5f, float octaveScale = 2.f, bool renormalize = true, unsigned int seed = 0);
float Compute3dPerlinNoise(float posX, float posY, float posZ, float scale = 1.f, unsigned int numOctaves = 1, float octavePersistence = 0.5f, float octaveScale = 2.f, bool renormalize = true, unsigned int seed = 0);
float Compute4dPerlinNoise(float posX, float posY, float posZ, float posT, float scale = 1.f, unsigned int numOctaves = 1, float octavePersistence = 0.5f, float octaveScale = 2.f, bool renormalize = true, unsigned int seed = 0);