#pragma once
#include "Engine/Math/Vector3.hpp"

class Sphere3D {
public:
	static const Sphere3D UNIT_SPHERE;
public:
	Vector3 center;
	float radius;
public:
	Sphere3D();
	~Sphere3D();
	Sphere3D(const Sphere3D& copy);
	explicit Sphere3D(float initialX, float initialY, float initialZ, float initialRadius);
	explicit Sphere3D(const Vector3& initialCenter, float initialRadius);
public:
	void StretchToIncludePoint(const Vector3& point);
	void AddPadding(float paddingRadius);
	void Translate(const Vector3& tranlsation);
public:
	bool IsPointInside(const Vector3& point) const;
public:
	const Sphere3D operator+(const Vector3& translation) const;
	const Sphere3D operator-(const Vector3& antiTranslation) const;
	void operator+=(const Vector3& translation);
	void operator-=(const Vector3& antiTranslation);
public:
	friend const Sphere3D Interpolate(const Sphere3D& start, const Sphere3D& end, float fractionToEnd);
};