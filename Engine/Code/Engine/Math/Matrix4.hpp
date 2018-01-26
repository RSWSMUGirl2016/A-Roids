#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/MathUtilities.hpp"

class Matrix4
{
public:
	Vector4 m_iBasis;
	Vector4 m_jBasis;
	Vector4 m_kBasis;
	Vector4 m_translation;
public:
	Matrix4();
	Matrix4(const Matrix4& copy);
	explicit Matrix4(const float* arrayOfFloats);
	explicit Matrix4(const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation = Vector2(0.f, 0.f));
	explicit Matrix4(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation = Vector3(0.f, 0.f, 0.f));
	explicit Matrix4(const Vector4& iBasis, const Vector4& jBasis, const Vector4& kBasis, const Vector4& translation = Vector4(0.f, 0.f, 0.f, 1.f));
public:
	const float* GetAsFloatArray() const;
	float* GetAsFloatArray();
	void MakeIdentity();
	void ConcatenateTransform(const Matrix4& other);
	Matrix4 GetTransformed(const Matrix4& other) const;
    Matrix4 GetInverse();
public:
	Vector2 TransformPosition(const Vector2& translation2D) const;
	Vector3 TransformPosition(const Vector3& translation3D) const;
	Vector2 TransformDirection(const Vector2& direction2D) const;
	Vector3 TransformDirection(const Vector3& direction3D) const;
	Vector4 TransformVector(const Vector4& homogeneousVector) const;
public:
	void Translate(const Vector2& translation2D);
	void Translate(const Vector3& translation3D);
    void TranslateAndTranspose(const Vector3& translation3D);
    void Scale(float uniformScale);
	void Scale(const Vector2& nonUniformScale2D);
	void Scale(const Vector3& nonUniformScale3D);
	void RotateDegreesAboutX(float degrees);
	void RotateDegreesAboutY(float degrees);
	void RotateDegreesAboutZ(float degrees);
	void RotateRadiansAboutX(float radians);
	void RotateRadiansAboutY(float radians);
	void RotateRadiansAboutZ(float radians);
public:
	static Matrix4 CreateTranslation(const Vector2& translation2D);
	static Matrix4 CreateTranslation(const Vector3& translation3D);
	static Matrix4 CreateScale(float uniformScale);
	static Matrix4 CreateScale(const Vector2& nonUniformScale2D);
	static Matrix4 CreateScale(const Vector3& nonUniformScale3D);
	static Matrix4 CreateRotateDegreesAboutX(float degrees);
	static Matrix4 CreateRotateDegreesAboutY(float degrees);
	static Matrix4 CreateRotateDegreesAboutZ(float degrees);
	static Matrix4 CreateRotateRadiansAboutX(float radians);
	static Matrix4 CreateRotateRadiansAboutY(float radians);
	static Matrix4 CreateRotateRadiansAboutZ(float radians);
    
    friend Matrix4 MakeOrthoProjection(float nx, float fx, float ny, float fy, float nz = 0.0f, float fz = 1.0f);
    friend Matrix4 MakePerspectiveProjection(float const fovRadians, float const aspectRatio, float const nz, float const fz);

    friend Vector4 MultiplyVector4ByMatrix4(Vector4 vector4, Matrix4 matrix);
    friend Vector3 MultiplyVector3ByMatrix4(Vector3 vector3, Matrix4 matrix);
    friend Vector2 MultiplyVector2ByMatrix4(Vector2 vector2, Matrix4 matrix);
    friend Matrix4 MultiplyMatrix4ByMatrix4(const Matrix4& lhs, const Matrix4& rhs);
    template <typename T> inline void Swap(T &a, T &b)
    {
        T temp = a;
        a = b;
        b = temp;
    };

    void transpose()
    {
        Swap(m_iBasis.y, m_jBasis.x);//[0][1], [1][0]
        Swap(m_iBasis.z, m_kBasis.x);//[0][2], [2][0]
        Swap(m_iBasis.w, m_translation.x);//[0][3], [3][0]
        Swap(m_jBasis.z, m_kBasis.y);//[1][2], [2][1]
        Swap(m_jBasis.w, m_translation.y);//[2][3], [3][1]
        Swap(m_kBasis.w, m_translation.z);//[2][3], [3][2]
    };

    Matrix4 get_transpose() const
    {
        Matrix4 ret = *this;
        ret.transpose();
        return ret;
    }

    Vector3 get_translation() const
    {
        return Vector3(m_translation.x, m_translation.y, m_translation.z);
    }

    void set_translation(Vector3 const & position)
    {
        m_translation = Vector4(position.x, position.y, position.z, 1.0f);
    }

    Vector4 get_column(unsigned int idx)
    {
        switch (idx)
        {
        case 0:
            return Vector4(m_iBasis.x, m_jBasis.x, m_kBasis.x, m_translation.x);
        case 1:
            return Vector4(m_iBasis.y, m_jBasis.y, m_kBasis.y, m_translation.y);
        case 2:
            return Vector4(m_iBasis.z, m_jBasis.z, m_kBasis.z, m_translation.z);
        case 3:
            return Vector4(m_iBasis.w, m_jBasis.w, m_kBasis.w, m_translation.w);
        default:
            return Vector4(m_iBasis.x, m_jBasis.x, m_kBasis.x, m_translation.x);
        }
        
    }

    Vector3 transform(Vector3 const &v)
    {
        Vector3 r;
        r.x = DotProduct(v, Vector3(get_column(0).x, get_column(0).y, get_column(0).z));
        r.y = DotProduct(v, Vector3(get_column(1).x, get_column(1).y, get_column(1).z));
        r.z = DotProduct(v, Vector3(get_column(2).x, get_column(2).y, get_column(2).z));
        return r;
    }

    void invert_orthonormal()
    {
        Vector3 translation = get_translation();
        set_translation(Vector3());

        transpose();

        translation = transform(translation);
        set_translation((-1) * translation);
    }

    Matrix4 get_inverse_orthonormal() const
    {
        Matrix4 ret = *this;
        ret.invert_orthonormal();
        return ret;
    }

    friend Matrix4 MatrixMakeLookAt(Vector3 const &position, Vector3 const& target, Vector3 const &worldUp = Vector3(0.0f, 1.0f, 0.0f));

public:
	void operator=(const Matrix4& copy);
};