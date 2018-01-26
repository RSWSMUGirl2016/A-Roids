#include "Engine/Math/Matrix4.hpp"
#include "Engine/Math/MathUtilities.hpp"

//*******************************************************************
Matrix4::Matrix4()
	: m_iBasis(1.f, 0.f, 0.f, 0.f)
	, m_jBasis(0.f, 1.f, 0.f, 0.f)
	, m_kBasis(0.f, 0.f, 1.f, 0.f)
	, m_translation(0.f, 0.f, 0.f, 1.f)
{
}

//*******************************************************************
Matrix4::Matrix4(const Matrix4& copy)
{
	m_iBasis = copy.m_iBasis;
	m_jBasis = copy.m_jBasis;
	m_kBasis = copy.m_kBasis;
	m_translation = copy.m_translation;
}

//*******************************************************************
Matrix4::Matrix4(const float* arrayOfFloats)
	: m_iBasis(arrayOfFloats[0], arrayOfFloats[1], arrayOfFloats[2], arrayOfFloats[3])
	, m_jBasis(arrayOfFloats[4], arrayOfFloats[5], arrayOfFloats[6], arrayOfFloats[7])
	, m_kBasis(arrayOfFloats[8], arrayOfFloats[9], arrayOfFloats[10], arrayOfFloats[11])
	, m_translation(arrayOfFloats[12], arrayOfFloats[13], arrayOfFloats[14], arrayOfFloats[15])
{
}

//*******************************************************************
Matrix4::Matrix4(const Vector2& iBasis, const Vector2& jBasis, const Vector2& translation)
	: m_iBasis(iBasis.x, iBasis.y, 0.f, 0.f)
	, m_jBasis(jBasis.x, jBasis.y, 0.f, 0.f)
	, m_kBasis(0.f, 0.f, 1.f, 0.f)
	, m_translation(translation.x, translation.y, 0.f, 1.f)
{
}

Matrix4::Matrix4(const Vector3& iBasis, const Vector3& jBasis, const Vector3& kBasis, const Vector3& translation /*= Vector3(0.f, 0.f, 0.f)*/)
	: m_iBasis(iBasis.x, iBasis.y, iBasis.z, 0.f)
	, m_jBasis(jBasis.x, jBasis.y, jBasis.z, 0.f)
	, m_kBasis(kBasis.x, kBasis.y, kBasis.z, 0.f)
	, m_translation(translation.x, translation.y, translation.z, 1.f)
{
}

//*******************************************************************
Matrix4::Matrix4(const Vector4& iBasis, const Vector4& jBasis, const Vector4& kBasis, const Vector4& translation /*= Vector4(0.f, 0.f, 0.f, 1.f)*/)
	: m_iBasis(iBasis * translation)
	, m_jBasis(jBasis * translation)
	, m_kBasis(kBasis * translation)
	, m_translation(translation)
{
}

//*******************************************************************
const float* Matrix4::GetAsFloatArray() const
{
	return &m_iBasis.x;
}

//*******************************************************************
float* Matrix4::GetAsFloatArray()
{
	return &m_iBasis.x;
}

//*******************************************************************
void Matrix4::MakeIdentity()
{
	m_iBasis = Vector4(1.f, 0.f, 0.f, 0.f);
	m_jBasis = Vector4(0.f, 1.f, 0.f, 0.f);
	m_kBasis = Vector4(0.f, 0.f, 1.f, 0.f);
	m_translation = Vector4(0.f, 0.f, 0.f, 1.f);
}

//*******************************************************************
void Matrix4::ConcatenateTransform(const Matrix4& other)
{
    Vector4 myI = m_iBasis;
    Vector4 myJ = m_jBasis;
    Vector4 myK = m_kBasis;
    Vector4 myT = m_translation;
    Vector4 myX = Vector4(m_iBasis.x, m_jBasis.x, m_kBasis.x, m_translation.x);
    Vector4 myY = Vector4(m_iBasis.y, m_jBasis.y, m_kBasis.y, m_translation.y);
    Vector4 myZ = Vector4(m_iBasis.z, m_jBasis.z, m_kBasis.z, m_translation.z);
    Vector4 myW = Vector4(m_iBasis.w, m_jBasis.w, m_kBasis.w, m_translation.w);

    Vector4 rhsI = other.m_iBasis;
    Vector4 rhsJ = other.m_jBasis;
    Vector4 rhsK = other.m_kBasis;
    Vector4 rhsT = other.m_translation;
    Vector4 rhsX = Vector4(other.m_iBasis.x, other.m_jBasis.x, other.m_kBasis.x, other.m_translation.x);
    Vector4 rhsY = Vector4(other.m_iBasis.y, other.m_jBasis.y, other.m_kBasis.y, other.m_translation.y);
    Vector4 rhsZ = Vector4(other.m_iBasis.z, other.m_jBasis.z, other.m_kBasis.z, other.m_translation.z);
    Vector4 rhsW = Vector4(other.m_iBasis.w, other.m_jBasis.w, other.m_kBasis.w, other.m_translation.w);

    float m00 = DotProduct(myI, rhsX); float m01 = DotProduct(myI, rhsY); float m02 = DotProduct(myI, rhsZ); float m03 = DotProduct(myI, rhsW);
    float m10 = DotProduct(myJ, rhsX); float m11 = DotProduct(myJ, rhsY); float m12 = DotProduct(myJ, rhsZ);; float m13 = DotProduct(myJ, rhsW);;
    float m20 = DotProduct(myK, rhsX); float m21 = DotProduct(myK, rhsY); float m22 = DotProduct(myK, rhsZ); float m23 = DotProduct(myK, rhsW);
    float m30 = DotProduct(myT, rhsX); float m31 = DotProduct(myT, rhsY); float m32 = DotProduct(myT, rhsZ); float m33 = DotProduct(myT, rhsW);

    m_iBasis = Vector4(m00, m01, m02, m03);
    m_jBasis = Vector4(m10, m11, m12, m13);
    m_kBasis = Vector4(m20, m21, m22, m23);
    m_translation = Vector4(m30, m31, m32, m33);
}

//*******************************************************************
Matrix4 Matrix4::GetTransformed(const Matrix4& other) const
{
	Matrix4 transform = Matrix4();

	Matrix4 oldMatrix = *this;

	transform.m_iBasis.x = (oldMatrix.m_iBasis.x * other.m_iBasis.x) + (oldMatrix.m_jBasis.x * other.m_iBasis.y) + (oldMatrix.m_kBasis.x * other.m_iBasis.z) + (oldMatrix.m_translation.x * other.m_iBasis.w);

	transform.m_jBasis.x = (oldMatrix.m_iBasis.x * other.m_jBasis.x) + (oldMatrix.m_jBasis.x * other.m_jBasis.y) + (oldMatrix.m_kBasis.x * other.m_jBasis.z) + (oldMatrix.m_translation.x * other.m_jBasis.w);

	transform.m_kBasis.x = (oldMatrix.m_iBasis.x * other.m_kBasis.x) + (oldMatrix.m_jBasis.x * other.m_kBasis.y) + (oldMatrix.m_kBasis.x * other.m_kBasis.z) + (oldMatrix.m_translation.x * other.m_kBasis.w);

	transform.m_translation.x = (oldMatrix.m_iBasis.x * other.m_translation.x) + (oldMatrix.m_jBasis.x * other.m_translation.y) + (oldMatrix.m_kBasis.x * other.m_translation.z) + (oldMatrix.m_translation.x * other.m_translation.w);



	transform.m_iBasis.y = (oldMatrix.m_iBasis.y * other.m_iBasis.x) + (oldMatrix.m_jBasis.y * other.m_iBasis.y) + (oldMatrix.m_kBasis.y * other.m_iBasis.z) + (oldMatrix.m_translation.y * other.m_iBasis.w);

	transform.m_jBasis.y = (oldMatrix.m_iBasis.y * other.m_jBasis.x) + (oldMatrix.m_jBasis.y * other.m_jBasis.y) + (oldMatrix.m_kBasis.y * other.m_jBasis.z) + (oldMatrix.m_translation.y * other.m_jBasis.w);

	transform.m_kBasis.y = (oldMatrix.m_iBasis.y * other.m_kBasis.x) + (oldMatrix.m_jBasis.y * other.m_kBasis.y) + (oldMatrix.m_kBasis.y * other.m_kBasis.z) + (oldMatrix.m_translation.y * other.m_kBasis.w);

	transform.m_translation.y = (oldMatrix.m_iBasis.y * other.m_translation.x) + (oldMatrix.m_jBasis.y * other.m_translation.y) + (oldMatrix.m_kBasis.y * other.m_translation.z) + (oldMatrix.m_translation.y * other.m_translation.w);



	transform.m_iBasis.z = (oldMatrix.m_iBasis.z * other.m_iBasis.x) + (oldMatrix.m_jBasis.z * other.m_iBasis.y) + (oldMatrix.m_kBasis.z * other.m_iBasis.z) + (oldMatrix.m_translation.z * other.m_iBasis.w);

	transform.m_jBasis.z = (oldMatrix.m_iBasis.z * other.m_jBasis.x) + (oldMatrix.m_jBasis.z * other.m_jBasis.y) + (oldMatrix.m_kBasis.z * other.m_jBasis.z) + (oldMatrix.m_translation.z * other.m_jBasis.w);

	transform.m_kBasis.z = (oldMatrix.m_iBasis.z * other.m_kBasis.x) + (oldMatrix.m_jBasis.z * other.m_kBasis.y) + (oldMatrix.m_kBasis.z * other.m_kBasis.z) + (oldMatrix.m_translation.z * other.m_kBasis.w);

	transform.m_translation.z = (oldMatrix.m_iBasis.z * other.m_translation.x) + (oldMatrix.m_jBasis.z * other.m_translation.y) + (oldMatrix.m_kBasis.z * other.m_translation.z) + (oldMatrix.m_translation.z * other.m_translation.w);



	transform.m_iBasis.w = (oldMatrix.m_iBasis.w * other.m_iBasis.x) + (oldMatrix.m_jBasis.w * other.m_iBasis.y) + (oldMatrix.m_kBasis.w * other.m_iBasis.z) + (oldMatrix.m_translation.w * other.m_iBasis.w);

	transform.m_jBasis.w = (oldMatrix.m_iBasis.w * other.m_jBasis.x) + (oldMatrix.m_jBasis.w * other.m_jBasis.y) + (oldMatrix.m_kBasis.w * other.m_jBasis.z) + (oldMatrix.m_translation.w * other.m_jBasis.w);

	transform.m_kBasis.w = (oldMatrix.m_iBasis.w * other.m_kBasis.x) + (oldMatrix.m_jBasis.w * other.m_kBasis.y) + (oldMatrix.m_kBasis.w * other.m_kBasis.z) + (oldMatrix.m_translation.w * other.m_kBasis.w);

	transform.m_translation.w = (oldMatrix.m_iBasis.w * other.m_translation.x) + (oldMatrix.m_jBasis.w * other.m_translation.y) + (oldMatrix.m_kBasis.w * other.m_translation.z) + (oldMatrix.m_translation.w * other.m_translation.w);


	return transform;
}

//*******************************************************************
Matrix4 Matrix4::GetInverse()
{
    float tempMatrix4[4][4] = {
                                    { this->m_iBasis.x, this->m_iBasis.y, this->m_iBasis.z, this->m_iBasis.w },
                                    { this->m_jBasis.x, this->m_jBasis.y, this->m_jBasis.z, this->m_jBasis.w },
                                    { this->m_kBasis.x, this->m_kBasis.y, this->m_kBasis.z, this->m_kBasis.w },
                                    { this->m_translation.x, this->m_translation.y, this->m_translation.z, this->m_translation.w }
                              };

    float detMatrix =
        (tempMatrix4[0][0] * tempMatrix4[1][1] * tempMatrix4[2][2] * tempMatrix4[3][3])
      + (tempMatrix4[0][0] * tempMatrix4[1][2] * tempMatrix4[2][3] * tempMatrix4[3][1])
      + (tempMatrix4[0][0] * tempMatrix4[1][3] * tempMatrix4[2][1] * tempMatrix4[3][2])
        
      + (tempMatrix4[0][1] * tempMatrix4[1][0] * tempMatrix4[2][3] * tempMatrix4[3][2])
      + (tempMatrix4[0][1] * tempMatrix4[1][2] * tempMatrix4[2][0] * tempMatrix4[3][3])
      + (tempMatrix4[0][1] * tempMatrix4[1][3] * tempMatrix4[2][2] * tempMatrix4[3][0])

      + (tempMatrix4[0][2] * tempMatrix4[1][0] * tempMatrix4[2][1] * tempMatrix4[3][3])
      + (tempMatrix4[0][2] * tempMatrix4[1][1] * tempMatrix4[2][3] * tempMatrix4[3][0])
      + (tempMatrix4[0][2] * tempMatrix4[1][3] * tempMatrix4[2][0] * tempMatrix4[3][1])

      + (tempMatrix4[0][3] * tempMatrix4[1][0] * tempMatrix4[2][2] * tempMatrix4[3][1])
      + (tempMatrix4[0][3] * tempMatrix4[1][1] * tempMatrix4[2][0] * tempMatrix4[3][2])
      + (tempMatrix4[0][3] * tempMatrix4[1][2] * tempMatrix4[2][1] * tempMatrix4[3][0])

      - (tempMatrix4[0][0] * tempMatrix4[1][1] * tempMatrix4[2][3] * tempMatrix4[3][2])
      - (tempMatrix4[0][0] * tempMatrix4[1][2] * tempMatrix4[2][1] * tempMatrix4[3][3])
      - (tempMatrix4[0][0] * tempMatrix4[1][3] * tempMatrix4[2][2] * tempMatrix4[3][1])

      - (tempMatrix4[0][1] * tempMatrix4[1][0] * tempMatrix4[2][2] * tempMatrix4[3][3])
      - (tempMatrix4[0][1] * tempMatrix4[1][2] * tempMatrix4[2][3] * tempMatrix4[3][0])
      - (tempMatrix4[0][1] * tempMatrix4[1][3] * tempMatrix4[2][0] * tempMatrix4[3][2])

      - (tempMatrix4[0][2] * tempMatrix4[1][0] * tempMatrix4[2][3] * tempMatrix4[3][1])
      - (tempMatrix4[0][2] * tempMatrix4[1][1] * tempMatrix4[2][0] * tempMatrix4[3][3])
      - (tempMatrix4[0][2] * tempMatrix4[1][3] * tempMatrix4[2][1] * tempMatrix4[3][0])

      - (tempMatrix4[0][3] * tempMatrix4[1][0] * tempMatrix4[2][1] * tempMatrix4[3][2])
      - (tempMatrix4[0][3] * tempMatrix4[1][1] * tempMatrix4[2][2] * tempMatrix4[3][0])
      - (tempMatrix4[0][3] * tempMatrix4[1][2] * tempMatrix4[2][0] * tempMatrix4[3][1])
      ;

    if (detMatrix == 0)
        return Matrix4();
     
    Matrix4 inverseMatrix = Matrix4();

    inverseMatrix.m_iBasis.x = 
             (1 / detMatrix) *
             ((tempMatrix4[1][1]*tempMatrix4[2][2]*tempMatrix4[3][3])
            + (tempMatrix4[1][2] * tempMatrix4[2][3] * tempMatrix4[3][1])
            + (tempMatrix4[1][3] * tempMatrix4[2][1] * tempMatrix4[3][2])
            - (tempMatrix4[1][1] * tempMatrix4[2][3] * tempMatrix4[3][2])
            - (tempMatrix4[1][2] * tempMatrix4[2][1] * tempMatrix4[3][3])
            - (tempMatrix4[1][3] * tempMatrix4[2][2] * tempMatrix4[3][1]));

    inverseMatrix.m_iBasis.y =
             (1 / detMatrix) *
             ((tempMatrix4[0][1] * tempMatrix4[2][3] * tempMatrix4[3][2])
            + (tempMatrix4[0][2] * tempMatrix4[2][1] * tempMatrix4[3][3])
            + (tempMatrix4[0][3] * tempMatrix4[2][2] * tempMatrix4[3][1])
            - (tempMatrix4[0][1] * tempMatrix4[2][2] * tempMatrix4[3][3])
            - (tempMatrix4[0][2] * tempMatrix4[2][3] * tempMatrix4[3][1])
            - (tempMatrix4[0][3] * tempMatrix4[2][1] * tempMatrix4[3][2]));

    inverseMatrix.m_iBasis.z =
             (1 / detMatrix) *
             ((tempMatrix4[0][1] * tempMatrix4[1][2] * tempMatrix4[3][3])
            + (tempMatrix4[0][2] * tempMatrix4[1][3] * tempMatrix4[3][1])
            + (tempMatrix4[0][3] * tempMatrix4[1][1] * tempMatrix4[3][2])
            - (tempMatrix4[0][1] * tempMatrix4[1][3] * tempMatrix4[3][2])
            - (tempMatrix4[0][2] * tempMatrix4[1][1] * tempMatrix4[3][3])
            - (tempMatrix4[0][3] * tempMatrix4[1][2] * tempMatrix4[3][1]));

    inverseMatrix.m_iBasis.w =
             (1 / detMatrix) *
             ((tempMatrix4[0][1] * tempMatrix4[1][3] * tempMatrix4[2][2])
            + (tempMatrix4[0][2] * tempMatrix4[1][1] * tempMatrix4[2][3])
            + (tempMatrix4[0][3] * tempMatrix4[1][2] * tempMatrix4[2][1])
            - (tempMatrix4[0][1] * tempMatrix4[1][2] * tempMatrix4[2][3])
            - (tempMatrix4[0][2] * tempMatrix4[1][3] * tempMatrix4[2][1])
            - (tempMatrix4[0][3] * tempMatrix4[1][1] * tempMatrix4[2][2]));

    inverseMatrix.m_jBasis.x =
             (1 / detMatrix) *
             ((tempMatrix4[1][0] * tempMatrix4[2][3] * tempMatrix4[3][2])
            + (tempMatrix4[1][2] * tempMatrix4[2][0] * tempMatrix4[3][3])
            + (tempMatrix4[1][3] * tempMatrix4[2][2] * tempMatrix4[3][0])
            - (tempMatrix4[1][0] * tempMatrix4[2][2] * tempMatrix4[3][3])
            - (tempMatrix4[1][2] * tempMatrix4[2][3] * tempMatrix4[3][0])
            - (tempMatrix4[1][3] * tempMatrix4[2][0] * tempMatrix4[3][2]));

    inverseMatrix.m_jBasis.y =
             (1 / detMatrix) *
             ((tempMatrix4[0][0] * tempMatrix4[2][2] * tempMatrix4[3][3])
            + (tempMatrix4[0][2] * tempMatrix4[2][3] * tempMatrix4[3][0])
            + (tempMatrix4[0][3] * tempMatrix4[2][0] * tempMatrix4[3][2])
            - (tempMatrix4[0][0] * tempMatrix4[2][3] * tempMatrix4[3][2])
            - (tempMatrix4[0][2] * tempMatrix4[2][0] * tempMatrix4[3][3])
            - (tempMatrix4[0][3] * tempMatrix4[2][2] * tempMatrix4[3][0]));

    inverseMatrix.m_jBasis.z =
             (1 / detMatrix) *
             ((tempMatrix4[0][0] * tempMatrix4[1][3] * tempMatrix4[3][2])
            + (tempMatrix4[0][2] * tempMatrix4[1][0] * tempMatrix4[3][3])
            + (tempMatrix4[0][3] * tempMatrix4[1][2] * tempMatrix4[3][0])
            - (tempMatrix4[0][0] * tempMatrix4[1][2] * tempMatrix4[3][3])
            - (tempMatrix4[0][2] * tempMatrix4[1][3] * tempMatrix4[3][0])
            - (tempMatrix4[0][3] * tempMatrix4[1][0] * tempMatrix4[3][2]));

    inverseMatrix.m_jBasis.w =
             (1 / detMatrix) *
             ((tempMatrix4[0][0] * tempMatrix4[1][2] * tempMatrix4[2][3])
            + (tempMatrix4[0][2] * tempMatrix4[1][3] * tempMatrix4[2][0])
            + (tempMatrix4[0][3] * tempMatrix4[1][0] * tempMatrix4[2][2])
            - (tempMatrix4[0][0] * tempMatrix4[1][3] * tempMatrix4[2][2])
            - (tempMatrix4[0][2] * tempMatrix4[1][0] * tempMatrix4[2][3])
            - (tempMatrix4[0][3] * tempMatrix4[1][2] * tempMatrix4[2][0]));

    inverseMatrix.m_kBasis.x =
             (1 / detMatrix) *
             ((tempMatrix4[1][0] * tempMatrix4[2][1] * tempMatrix4[3][3])
            + (tempMatrix4[1][1] * tempMatrix4[2][3] * tempMatrix4[3][0])
            + (tempMatrix4[1][3] * tempMatrix4[2][0] * tempMatrix4[3][1])
            - (tempMatrix4[1][0] * tempMatrix4[2][3] * tempMatrix4[3][1])
            - (tempMatrix4[1][1] * tempMatrix4[2][0] * tempMatrix4[3][3])
            - (tempMatrix4[1][3] * tempMatrix4[2][1] * tempMatrix4[3][0]));

    inverseMatrix.m_kBasis.y =
             (1 / detMatrix) *
             ((tempMatrix4[0][0] * tempMatrix4[2][3] * tempMatrix4[3][1])
            + (tempMatrix4[0][1] * tempMatrix4[2][0] * tempMatrix4[3][3])
            + (tempMatrix4[0][3] * tempMatrix4[2][1] * tempMatrix4[3][0])
            - (tempMatrix4[0][0] * tempMatrix4[2][1] * tempMatrix4[3][3])
            - (tempMatrix4[0][1] * tempMatrix4[2][3] * tempMatrix4[3][0])
            - (tempMatrix4[0][3] * tempMatrix4[2][0] * tempMatrix4[3][1]));

    inverseMatrix.m_kBasis.z =
             (1 / detMatrix) *
             ((tempMatrix4[0][0] * tempMatrix4[1][1] * tempMatrix4[3][3])
            + (tempMatrix4[0][1] * tempMatrix4[1][3] * tempMatrix4[3][0])
            + (tempMatrix4[0][3] * tempMatrix4[1][0] * tempMatrix4[3][1])
            - (tempMatrix4[0][0] * tempMatrix4[1][3] * tempMatrix4[3][1])
            - (tempMatrix4[0][1] * tempMatrix4[1][0] * tempMatrix4[3][3])
            - (tempMatrix4[0][3] * tempMatrix4[1][1] * tempMatrix4[3][0]));

    inverseMatrix.m_kBasis.w =
             (1 / detMatrix) *
             ((tempMatrix4[0][0] * tempMatrix4[1][3] * tempMatrix4[2][1])
            + (tempMatrix4[0][1] * tempMatrix4[1][0] * tempMatrix4[2][3])
            + (tempMatrix4[0][3] * tempMatrix4[1][1] * tempMatrix4[2][0])
            - (tempMatrix4[0][0] * tempMatrix4[1][1] * tempMatrix4[2][3])
            - (tempMatrix4[0][1] * tempMatrix4[1][3] * tempMatrix4[2][0])
            - (tempMatrix4[0][3] * tempMatrix4[1][0] * tempMatrix4[2][1]));

    inverseMatrix.m_translation.x =
             (1 / detMatrix) *
             ((tempMatrix4[1][0] * tempMatrix4[2][2] * tempMatrix4[3][1])
            + (tempMatrix4[1][1] * tempMatrix4[2][0] * tempMatrix4[3][2])
            + (tempMatrix4[1][2] * tempMatrix4[2][1] * tempMatrix4[3][0])
            - (tempMatrix4[1][0] * tempMatrix4[2][1] * tempMatrix4[3][2])
            - (tempMatrix4[1][1] * tempMatrix4[2][2] * tempMatrix4[3][0])
            - (tempMatrix4[1][2] * tempMatrix4[2][0] * tempMatrix4[3][1]));

    inverseMatrix.m_translation.y =
             (1 / detMatrix) *
             ((tempMatrix4[0][0] * tempMatrix4[2][1] * tempMatrix4[3][2])
            + (tempMatrix4[0][1] * tempMatrix4[2][2] * tempMatrix4[3][0])
            + (tempMatrix4[0][2] * tempMatrix4[2][0] * tempMatrix4[3][1])
            - (tempMatrix4[0][0] * tempMatrix4[2][2] * tempMatrix4[3][1])
            - (tempMatrix4[0][1] * tempMatrix4[2][0] * tempMatrix4[3][2])
            - (tempMatrix4[0][2] * tempMatrix4[2][1] * tempMatrix4[3][0]));

    inverseMatrix.m_translation.z =
             (1 / detMatrix) *
             ((tempMatrix4[0][0] * tempMatrix4[1][2] * tempMatrix4[3][1])
            + (tempMatrix4[0][1] * tempMatrix4[1][0] * tempMatrix4[3][2])
            + (tempMatrix4[0][2] * tempMatrix4[1][1] * tempMatrix4[3][0])
            - (tempMatrix4[0][0] * tempMatrix4[1][1] * tempMatrix4[3][2])
            - (tempMatrix4[0][1] * tempMatrix4[1][2] * tempMatrix4[3][0])
            - (tempMatrix4[0][2] * tempMatrix4[1][0] * tempMatrix4[3][1]));

    inverseMatrix.m_translation.w =
             (1 / detMatrix) *
             ((tempMatrix4[0][0] * tempMatrix4[1][1] * tempMatrix4[2][2])
            + (tempMatrix4[0][1] * tempMatrix4[1][2] * tempMatrix4[2][0])
            + (tempMatrix4[0][2] * tempMatrix4[1][0] * tempMatrix4[2][1])
            - (tempMatrix4[0][0] * tempMatrix4[1][2] * tempMatrix4[2][1])
            - (tempMatrix4[0][1] * tempMatrix4[1][0] * tempMatrix4[2][2])
            - (tempMatrix4[0][2] * tempMatrix4[1][1] * tempMatrix4[2][0]));

    return inverseMatrix;
}

//*******************************************************************
Vector2 Matrix4::TransformPosition(const Vector2& translation2D) const
{
	Vector2 newPosition = Vector2();

	newPosition.x = (m_iBasis.x*translation2D.x) + (m_jBasis.x*translation2D.y) + (m_kBasis.x*0.f) + (m_translation.x*1.f);

	newPosition.y = (m_iBasis.y*translation2D.x) + (m_jBasis.y*translation2D.y) + (m_kBasis.y*0.f) + (m_translation.y*1.f);

	/*newPosition.z = (m_iBasis.z*translation2D.x) + (m_jBasis.z*translation2D.y) + (m_kBasis.z*0.f) + (m_translation.z*1.f);

	newPosition.w = (m_iBasis.w*translation2D.x) + (m_jBasis.w*translation2D.y) + (m_kBasis.w*0.f) + (m_translation.w*1.f);*/

	return newPosition;
}

//*******************************************************************
Vector3 Matrix4::TransformPosition(const Vector3& translation3D) const
{
	Vector3 newPosition = Vector3();

	newPosition.x = (m_iBasis.x*translation3D.x) + (m_jBasis.x*translation3D.y) + (m_kBasis.x*translation3D.z) + (m_translation.x*1.f);

	newPosition.y = (m_iBasis.y*translation3D.x) + (m_jBasis.y*translation3D.y) + (m_kBasis.y*translation3D.z) + (m_translation.y*1.f);

	newPosition.z = (m_iBasis.z*translation3D.x) + (m_jBasis.z*translation3D.y) + (m_kBasis.z*translation3D.z) + (m_translation.z*1.f);

	/*newPosition.w = (m_iBasis.w*translation3D.x) + (m_jBasis.w*translation3D.y) + (m_kBasis.w*translation3D.z) + (m_translation.w*1.f);*/

	return newPosition;
}

//*******************************************************************
Vector2 Matrix4::TransformDirection(const Vector2& direction2D) const
{
	Vector2 newDirection = Vector2();

	newDirection.x = (m_iBasis.x*direction2D.x) + (m_jBasis.x*direction2D.y) + (m_kBasis.x*0.f) + (m_translation.x*0.f);

	newDirection.y = (m_iBasis.y*direction2D.x) + (m_jBasis.y*direction2D.y) + (m_kBasis.y*0.f) + (m_translation.y*0.f);

	/*newPosition.z = (m_iBasis.z*translation2D.x) + (m_jBasis.z*translation2D.y) + (m_kBasis.z*0.f) + (m_translation.z*1.f);

	newPosition.w = (m_iBasis.w*translation2D.x) + (m_jBasis.w*translation2D.y) + (m_kBasis.w*0.f) + (m_translation.w*1.f);*/

	return newDirection;
}

//*******************************************************************
Vector3 Matrix4::TransformDirection(const Vector3& direction3D) const
{
	Vector3 newDirection = Vector3();

	newDirection.x = (m_iBasis.x*direction3D.x) + (m_jBasis.x*direction3D.y) + (m_kBasis.x*direction3D.z) + (m_translation.x*0.f);

	newDirection.y = (m_iBasis.y*direction3D.x) + (m_jBasis.y*direction3D.y) + (m_kBasis.y*direction3D.z) + (m_translation.y*0.f);

	newDirection.z = (m_iBasis.z*direction3D.x) + (m_jBasis.z*direction3D.y) + (m_kBasis.z*direction3D.z) + (m_translation.z*0.f);

	/*newPosition.w = (m_iBasis.w*translation3D.x) + (m_jBasis.w*translation3D.y) + (m_kBasis.w*translation3D.z) + (m_translation.w*1.f);*/

	return newDirection;
}

//*******************************************************************
Vector4 Matrix4::TransformVector(const Vector4& homogeneousVector) const
{
	Vector4 newVector = Vector4();

	newVector.x = (m_iBasis.x*homogeneousVector.x) + (m_jBasis.x*homogeneousVector.y) + (m_kBasis.x*homogeneousVector.z) + (m_translation.x*homogeneousVector.w);

	newVector.y = (m_iBasis.y*homogeneousVector.x) + (m_jBasis.y*homogeneousVector.y) + (m_kBasis.y*homogeneousVector.z) + (m_translation.y*homogeneousVector.w);

	newVector.z = (m_iBasis.z*homogeneousVector.x) + (m_jBasis.z*homogeneousVector.y) + (m_kBasis.z*homogeneousVector.z) + (m_translation.z*homogeneousVector.w);

	newVector.w = (m_iBasis.w*homogeneousVector.x) + (m_jBasis.w*homogeneousVector.y) + (m_kBasis.w*homogeneousVector.z) + (m_translation.w*homogeneousVector.w);

	return newVector;
}

//*******************************************************************
void Matrix4::Translate(const Vector2& translation2D)
{
	Matrix4 matrixTranslation2D = Matrix4();
	matrixTranslation2D.m_translation.x = translation2D.x;
	matrixTranslation2D.m_translation.y = translation2D.y;
	
	this->ConcatenateTransform(matrixTranslation2D);
}

//*******************************************************************
void Matrix4::Translate(const Vector3& translation3D)
{
	Matrix4 matrixTranslation3D = Matrix4();
	matrixTranslation3D.m_translation.x = translation3D.x;
	matrixTranslation3D.m_translation.y = translation3D.y;
	matrixTranslation3D.m_translation.z = translation3D.z;

	this->ConcatenateTransform(matrixTranslation3D);
}

//*******************************************************************
void Matrix4::TranslateAndTranspose(const Vector3& translation3D)
{
    Matrix4 matrixTranslation3D = Matrix4();
    matrixTranslation3D.m_translation.x = translation3D.x;
    matrixTranslation3D.m_translation.y = translation3D.y;
    matrixTranslation3D.m_translation.z = translation3D.z;

    this->ConcatenateTransform(matrixTranslation3D.get_transpose());
}

//*******************************************************************
void Matrix4::Scale(float uniformScale)
{
	Matrix4 matrixScale = Matrix4();
	matrixScale.m_iBasis.x = uniformScale;
	matrixScale.m_jBasis.y = uniformScale;
	matrixScale.m_kBasis.z = uniformScale;

	this->ConcatenateTransform(matrixScale);
}

//*******************************************************************
void Matrix4::Scale(const Vector2& nonUniformScale2D)
{
	Matrix4 matrixNonUniformScale2D = Matrix4();
	matrixNonUniformScale2D.m_iBasis.x = nonUniformScale2D.x;
	matrixNonUniformScale2D.m_jBasis.y = nonUniformScale2D.y;

	this->ConcatenateTransform(matrixNonUniformScale2D);
}

//*******************************************************************
void Matrix4::Scale(const Vector3& nonUniformScale3D)
{
	Matrix4 matrixNonUniformScale3D = Matrix4();
	matrixNonUniformScale3D.m_iBasis.x = nonUniformScale3D.x;
	matrixNonUniformScale3D.m_jBasis.y = nonUniformScale3D.y;
	matrixNonUniformScale3D.m_kBasis.z = nonUniformScale3D.z;

	this->ConcatenateTransform(matrixNonUniformScale3D);
}

//*******************************************************************
void Matrix4::RotateDegreesAboutX(float degrees)
{
	Matrix4 matrixRotateAboutX = Matrix4();
	matrixRotateAboutX.m_jBasis.y = CosDegrees(degrees);
	matrixRotateAboutX.m_jBasis.z = SinDegrees(degrees);
	matrixRotateAboutX.m_kBasis.y = -1.f*SinDegrees(degrees);
	matrixRotateAboutX.m_kBasis.z = CosDegrees(degrees);

	this->ConcatenateTransform(matrixRotateAboutX);
}

//*******************************************************************
void Matrix4::RotateDegreesAboutY(float degrees)
{
	Matrix4 matrixRotateAboutY = Matrix4();
	matrixRotateAboutY.m_iBasis.x = CosDegrees(degrees);
	matrixRotateAboutY.m_iBasis.z = -1.f*SinDegrees(degrees);
	matrixRotateAboutY.m_kBasis.x = SinDegrees(degrees);
	matrixRotateAboutY.m_kBasis.z = CosDegrees(degrees);

	this->ConcatenateTransform(matrixRotateAboutY);
}

//*******************************************************************
void Matrix4::RotateDegreesAboutZ(float degrees)
{
	Matrix4 matrixRotateAboutZ = Matrix4();
	matrixRotateAboutZ.m_iBasis.x = CosDegrees(degrees);
	matrixRotateAboutZ.m_iBasis.y = SinDegrees(degrees);
	matrixRotateAboutZ.m_jBasis.x = -1.f*SinDegrees(degrees);
	matrixRotateAboutZ.m_jBasis.y = CosDegrees(degrees);

    this->ConcatenateTransform(matrixRotateAboutZ);
}

//*******************************************************************
void Matrix4::RotateRadiansAboutX(float radians)
{
	Matrix4 matrixRotateAboutX = Matrix4();
	matrixRotateAboutX.m_jBasis.y = (float) cos(radians);
	matrixRotateAboutX.m_jBasis.z = (float)sin(radians);
	matrixRotateAboutX.m_kBasis.y = -1.f*(float)sin(radians);
	matrixRotateAboutX.m_kBasis.z = (float)cos(radians);

	this->ConcatenateTransform(matrixRotateAboutX);
}

//*******************************************************************
void Matrix4::RotateRadiansAboutY(float radians)
{
	Matrix4 matrixRotateAboutY = Matrix4();
	matrixRotateAboutY.m_iBasis.x = (float)cos(radians);
	matrixRotateAboutY.m_iBasis.z = -1.f*(float)sin(radians);
	matrixRotateAboutY.m_kBasis.x = (float)sin(radians);
	matrixRotateAboutY.m_kBasis.z = (float)cos(radians);

	this->ConcatenateTransform(matrixRotateAboutY);
}

//*******************************************************************
void Matrix4::RotateRadiansAboutZ(float radians)
{
	Matrix4 matrixRotateAboutZ = Matrix4();
	matrixRotateAboutZ.m_iBasis.x = (float)cos(radians);
	matrixRotateAboutZ.m_iBasis.y = (float)sin(radians);
	matrixRotateAboutZ.m_jBasis.x = -1.f*(float)sin(radians);
	matrixRotateAboutZ.m_jBasis.y = (float)cos(radians);

	this->ConcatenateTransform(matrixRotateAboutZ);
}

//*******************************************************************
Matrix4 Matrix4::CreateTranslation(const Vector2& translation2D)
{
	Matrix4 matrixTranslation2D = Matrix4();
	matrixTranslation2D.m_translation.x = translation2D.x;
	matrixTranslation2D.m_translation.y = translation2D.y;
	return matrixTranslation2D;
}

//*******************************************************************
Matrix4 Matrix4::CreateTranslation(const Vector3& translation3D)
{
	Matrix4 matrixTranslation3D = Matrix4();
	matrixTranslation3D.m_translation.x = translation3D.x;
	matrixTranslation3D.m_translation.y = translation3D.y;
	matrixTranslation3D.m_translation.z = translation3D.z;
	return matrixTranslation3D;
}

//*******************************************************************
Matrix4 Matrix4::CreateScale(float uniformScale)
{
	Matrix4 matrixScale = Matrix4();
	matrixScale.m_iBasis.x = uniformScale;
	matrixScale.m_jBasis.y = uniformScale;
	matrixScale.m_kBasis.z = uniformScale;
	return matrixScale;
}

//*******************************************************************
Matrix4 Matrix4::CreateScale(const Vector2& nonUniformScale2D)
{
	Matrix4 matrixNonUniformScale2D = Matrix4();
	matrixNonUniformScale2D.m_iBasis.x = nonUniformScale2D.x;
	matrixNonUniformScale2D.m_jBasis.y = nonUniformScale2D.y;
	return matrixNonUniformScale2D;
}

//*******************************************************************
Matrix4 Matrix4::CreateScale(const Vector3& nonUniformScale3D)
{
	Matrix4 matrixNonUniformScale3D = Matrix4();
	matrixNonUniformScale3D.m_iBasis.x = nonUniformScale3D.x;
	matrixNonUniformScale3D.m_jBasis.y = nonUniformScale3D.y;
	matrixNonUniformScale3D.m_kBasis.z = nonUniformScale3D.z;
	return matrixNonUniformScale3D;
}

//*******************************************************************
Matrix4 Matrix4::CreateRotateDegreesAboutX(float degrees)
{
	Matrix4 matrixRotateAboutX = Matrix4();
	matrixRotateAboutX.m_jBasis.y = CosDegrees(degrees);
	matrixRotateAboutX.m_jBasis.z = SinDegrees(degrees);
	matrixRotateAboutX.m_kBasis.y = -1.f*SinDegrees(degrees);
	matrixRotateAboutX.m_kBasis.z = CosDegrees(degrees);
	return matrixRotateAboutX;
}

//*******************************************************************
Matrix4 Matrix4::CreateRotateDegreesAboutY(float degrees)
{
	Matrix4 matrixRotateAboutY = Matrix4();
	matrixRotateAboutY.m_iBasis.x = CosDegrees(degrees);
	matrixRotateAboutY.m_iBasis.z = -1.f*SinDegrees(degrees);
	matrixRotateAboutY.m_kBasis.x = SinDegrees(degrees);
	matrixRotateAboutY.m_kBasis.z = CosDegrees(degrees);
	return matrixRotateAboutY;
}

//*******************************************************************
Matrix4 Matrix4::CreateRotateDegreesAboutZ(float degrees)
{
	Matrix4 matrixRotateAboutZ = Matrix4();
	matrixRotateAboutZ.m_iBasis.x = CosDegrees(degrees);
	matrixRotateAboutZ.m_iBasis.y = SinDegrees(degrees);
	matrixRotateAboutZ.m_jBasis.x = -1.f*SinDegrees(degrees);
	matrixRotateAboutZ.m_jBasis.y = CosDegrees(degrees);
	return matrixRotateAboutZ;
}

//*******************************************************************
Matrix4 Matrix4::CreateRotateRadiansAboutX(float radians)
{
	Matrix4 matrixRotateAboutX = Matrix4();
	matrixRotateAboutX.m_jBasis.y = (float)cos(radians);
	matrixRotateAboutX.m_jBasis.z = (float)sin(radians);
	matrixRotateAboutX.m_kBasis.y = -1.f*(float)sin(radians);
	matrixRotateAboutX.m_kBasis.z = (float)cos(radians);
	return matrixRotateAboutX;
}

//*******************************************************************
Matrix4 Matrix4::CreateRotateRadiansAboutY(float radians)
{
	Matrix4 matrixRotateAboutY = Matrix4();
	matrixRotateAboutY.m_iBasis.x = (float)cos(radians);
	matrixRotateAboutY.m_iBasis.z = -1.f*(float)sin(radians);
	matrixRotateAboutY.m_kBasis.x = (float)sin(radians);
	matrixRotateAboutY.m_kBasis.z = (float)cos(radians);
	return matrixRotateAboutY;
}

//*******************************************************************
Matrix4 Matrix4::CreateRotateRadiansAboutZ(float radians)
{
	Matrix4 matrixRotateAboutZ = Matrix4();
	matrixRotateAboutZ.m_iBasis.x = (float)cos(radians);
	matrixRotateAboutZ.m_iBasis.y = (float)sin(radians);
	matrixRotateAboutZ.m_jBasis.x = -1.f*(float)sin(radians);
	matrixRotateAboutZ.m_jBasis.y = (float)cos(radians);
	return matrixRotateAboutZ;
}

//*******************************************************************
Matrix4 MatrixMakeLookAt(Vector3 const &position, Vector3 const& target, Vector3 const &worldUp)
{
    Vector3 direction = target - position;
    Vector3 forward = direction.GetNormalized();
    Vector3 right = worldUp.CrossProduct(forward);
    Vector3 up = forward.CrossProduct(right);

    Matrix4 mat;
    mat.m_iBasis = Vector4(right.x, right.y, right.z, 0.f);
    mat.m_jBasis = Vector4(up.x, up.y, up.z, 0.f);
    mat.m_kBasis = Vector4(forward.x, forward.y, forward.z, 0.f);
    mat.m_translation = Vector4(position.x, position.y, position.z, 1.f);

    return mat;
}

//*******************************************************************
Matrix4 MakeOrthoProjection(float nx, float fx, float ny, float fy, float nz /*= 0.0f*/, float fz /*= 1.0f*/)
{
    Matrix4 matrix4;

    float sx = 1.0f / (fx - nx);
    float sy = 1.0f / (fy - ny);
    float sz = 1.0f / (fz - nz);

    matrix4.m_iBasis = Vector4(2.0f * sx, 0.0f, 0.0f, 0.0f);
    matrix4.m_jBasis = Vector4(0.0f, 2.0f * sy, 0.0f, 0.0f);
    matrix4.m_kBasis = Vector4(0.0f, 0.0f, 1.0f * sz, 0.0f);
    matrix4.m_translation = Vector4(-(fx + nx) * sx, -(fy + ny) * sy, -nz * sz, 1.0f);

    return matrix4;
}

//*******************************************************************
Matrix4 MakePerspectiveProjection(float const fovRadians, float const aspectRatio, float const nz, float const fz)
{
    float size = 1.0f / tanf(fovRadians / 2.0f);

    // scale X or Y depending which dimension is bigger
    float w = size;
    float h = size;
    if (aspectRatio > 1.0f)
    {
        w /= aspectRatio;
    }
    else 
    {
        h *= aspectRatio;
    }

    float q = 1.0f / (fz - nz);

    Matrix4 matrix4;
    matrix4.m_iBasis = Vector4(w, 0.0f, 0.0f, 0.0f);
    matrix4.m_jBasis = Vector4(0.0f, h, 0.0f, 0.0f);
    matrix4.m_kBasis = Vector4(0.0f, 0.0f, fz * q, 1.0f);
    matrix4.m_translation = Vector4(0.0f, 0.0f, -nz * fz * q, 0.0f);

    return matrix4;
}

//*******************************************************************
Vector4 MultiplyVector4ByMatrix4(Vector4 vector4, Matrix4 matrix)
{
    /*Vector4 resultVector = Vector4();
    resultVector.x = vector4.x*matrix.m_iBasis.x + vector4.y*matrix.m_iBasis.y + vector4.z*matrix.m_iBasis.z + vector4.w*matrix.m_iBasis.w;
    resultVector.y = vector4.x*matrix.m_jBasis.x + vector4.y*matrix.m_jBasis.y + vector4.z*matrix.m_jBasis.z + vector4.w*matrix.m_jBasis.w;
    resultVector.z = vector4.x*matrix.m_kBasis.x + vector4.y*matrix.m_kBasis.y + vector4.z*matrix.m_kBasis.z + vector4.w*matrix.m_kBasis.w;
    resultVector.w = vector4.x*matrix.m_translation.x + vector4.y*matrix.m_translation.y + vector4.z*matrix.m_translation.z + vector4.w*matrix.m_translation.w;
    return resultVector;*/

    Vector4 resultVector(vector4.x, vector4.y, vector4.z, vector4.w);
    Vector4 matrixX = Vector4(matrix.m_iBasis.x, matrix.m_jBasis.x, matrix.m_kBasis.x, matrix.m_translation.x);
    Vector4 matrixY = Vector4(matrix.m_iBasis.y, matrix.m_jBasis.y, matrix.m_kBasis.y, matrix.m_translation.y);
    Vector4 matrixZ = Vector4(matrix.m_iBasis.z, matrix.m_jBasis.z, matrix.m_kBasis.z, matrix.m_translation.z);
    Vector4 matrixW = Vector4(matrix.m_iBasis.w, matrix.m_jBasis.w, matrix.m_kBasis.w, matrix.m_translation.w);


    return Vector4(DotProduct(resultVector, matrixX), DotProduct(resultVector, matrixY), DotProduct(resultVector, matrixZ), DotProduct(resultVector, matrixW));
}

//*******************************************************************
Vector3 MultiplyVector3ByMatrix4(Vector3 vector3, Matrix4 matrix)
{
    Vector4 resultVector(vector3.x, vector3.y, vector3.z, 1.0f);
    Vector4 matrixX = Vector4(matrix.m_iBasis.x, matrix.m_jBasis.x, matrix.m_kBasis.x, matrix.m_translation.x);
    Vector4 matrixY = Vector4(matrix.m_iBasis.y, matrix.m_jBasis.y, matrix.m_kBasis.y, matrix.m_translation.y);
    Vector4 matrixZ = Vector4(matrix.m_iBasis.z, matrix.m_jBasis.z, matrix.m_kBasis.z, matrix.m_translation.z);

    return Vector3(DotProduct(resultVector, matrixX), DotProduct(resultVector, matrixY), DotProduct(resultVector, matrixZ));
}

//*******************************************************************
Vector2 MultiplyVector2ByMatrix4(Vector2 vector2, Matrix4 matrix)
{
    Vector4 resultVector(vector2.x, vector2.y, 0.0f, 1.0f);
    Vector4 matrixX = Vector4(matrix.m_iBasis.x, matrix.m_jBasis.x, matrix.m_kBasis.x, matrix.m_translation.x);
    Vector4 matrixY = Vector4(matrix.m_iBasis.y, matrix.m_jBasis.y, matrix.m_kBasis.y, matrix.m_translation.y);
    Vector4 matrixZ = Vector4(matrix.m_iBasis.z, matrix.m_jBasis.z, matrix.m_kBasis.z, matrix.m_translation.z);

    return Vector2(DotProduct(resultVector, matrixX), DotProduct(resultVector, matrixY));
}

//*******************************************************************
//Row-Major Multiplication
Matrix4 MultiplyMatrix4ByMatrix4(const Matrix4& lhs, const Matrix4& rhs)
{
    Matrix4 resultMatrix = Matrix4();

    Vector4 myI = lhs.m_iBasis;
    Vector4 myJ = lhs.m_jBasis;
    Vector4 myK = lhs.m_kBasis;
    Vector4 myT = lhs.m_translation;
    Vector4 myX = Vector4(lhs.m_iBasis.x, lhs.m_jBasis.x, lhs.m_kBasis.x, lhs.m_translation.x);
    Vector4 myY = Vector4(lhs.m_iBasis.y, lhs.m_jBasis.y, lhs.m_kBasis.y, lhs.m_translation.y);
    Vector4 myZ = Vector4(lhs.m_iBasis.z, lhs.m_jBasis.z, lhs.m_kBasis.z, lhs.m_translation.z);
    Vector4 myW = Vector4(lhs.m_iBasis.w, lhs.m_jBasis.w, lhs.m_kBasis.w, lhs.m_translation.w);

    Vector4 rhsI = rhs.m_iBasis;
    Vector4 rhsJ = rhs.m_jBasis;
    Vector4 rhsK = rhs.m_kBasis;
    Vector4 rhsT = rhs.m_translation;
    Vector4 rhsX = Vector4(rhs.m_iBasis.x, rhs.m_jBasis.x, rhs.m_kBasis.x, rhs.m_translation.x);
    Vector4 rhsY = Vector4(rhs.m_iBasis.y, rhs.m_jBasis.y, rhs.m_kBasis.y, rhs.m_translation.y);
    Vector4 rhsZ = Vector4(rhs.m_iBasis.z, rhs.m_jBasis.z, rhs.m_kBasis.z, rhs.m_translation.z);
    Vector4 rhsW = Vector4(rhs.m_iBasis.w, rhs.m_jBasis.w, rhs.m_kBasis.w, rhs.m_translation.w);

    float m00 = DotProduct(myI, rhsX); float m01 = DotProduct(myI, rhsY); float m02 = DotProduct(myI, rhsZ); float m03 = DotProduct(myI, rhsW);
    float m10 = DotProduct(myJ, rhsX); float m11 = DotProduct(myJ, rhsY); float m12 = DotProduct(myJ, rhsZ);; float m13 = DotProduct(myJ, rhsW);;
    float m20 = DotProduct(myK, rhsX); float m21 = DotProduct(myK, rhsY); float m22 = DotProduct(myK, rhsZ); float m23 = DotProduct(myK, rhsW);
    float m30 = DotProduct(myT, rhsX); float m31 = DotProduct(myT, rhsY); float m32 = DotProduct(myT, rhsZ); float m33 = DotProduct(myT, rhsW);

    resultMatrix.m_iBasis = Vector4(m00, m01, m02, m03);
    resultMatrix.m_jBasis = Vector4(m10, m11, m12, m13);
    resultMatrix.m_kBasis = Vector4(m20, m21, m22, m23);
    resultMatrix.m_translation = Vector4(m30, m31, m32, m33);

    return resultMatrix;
}

//*******************************************************************
void Matrix4::operator=(const Matrix4& copy)
{
	m_iBasis = copy.m_iBasis;
	m_jBasis = copy.m_jBasis;
	m_kBasis = copy.m_kBasis;
	m_translation = copy.m_translation;
}

