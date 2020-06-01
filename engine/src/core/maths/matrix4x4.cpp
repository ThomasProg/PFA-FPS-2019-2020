#include <cassert>
#include <string.h>
#include <cmath>

#include "matrix4x4.hpp"
#include "transform.hpp"

#include "vec3.hpp"
#include "vec4.hpp"
// #include "transform.hpp"

using namespace Core::Maths;

Matrix4x4::Matrix4x4() 
{
    this->nbLines   = 4;
    this->nbColumns = 4;

    this->matrix = new float[nbLines * nbColumns];

    for (unsigned int i = 0; i < this->nbColumns * this->nbLines; i++)
        matrix[i] = 0;
    //set identity
    for (unsigned int i = 0; i < this->nbColumns; i++)
    {
        matrix[i + nbColumns * i] = 1;
    }
}

Matrix4x4::Matrix4x4 (const Matrix4x4& copiedMatrix) 
    //: Matrix(copiedMatrix)
{
    this->nbColumns = copiedMatrix.nbColumns;
    this->nbLines   = copiedMatrix.nbLines;
    this->matrix    = new float[this->nbLines * this->nbColumns];
    
    memcpy(this->matrix, copiedMatrix.matrix, sizeof(float) * this->nbColumns * this->nbLines);

    // LOGIC_ASSERT(copiedMatrix.nbLines == 4 && copiedMatrix.nbColumns == 4, "Matrix should not be 4x4 if movedMatrix is not.");
}

Matrix4x4::Matrix4x4(Matrix&& movedMatrix)
    : Matrix(std::move(movedMatrix))
{
    // LOGIC_ASSERT(movedMatrix.nbLines == 4 && movedMatrix.nbColumns == 4, "Matrix should not be 4x4 if movedMatrix is not.");
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& rhs)
{
    // LOGIC_ASSERT(rhs.matrix != nullptr, 
    //             "The copied matrix should not be invalid!");

    // LOGIC_ASSERT(this->nbColumns == rhs.nbColumns && this->nbLines == rhs.nbLines, 
    //             "Height and Width should already be the same if they are both Matrix4x4");

    if (this->matrix != nullptr)
        delete[] this->matrix;

    this->matrix = new float[nbElements];

    for (unsigned int i = 0; i < rhs.nbElements; ++i)
    {
        this->matrix[i] = rhs.matrix[i];
    }

    return *this;
}

Matrix4x4& Matrix4x4::operator=(Matrix4x4&& rhs)
{
    if (this->matrix != nullptr)
        delete[] this->matrix;

    this->matrix = rhs.matrix; 
    rhs.matrix   = nullptr;

    // LOGIC_ASSERT(this->nbColumns == rhs.nbColumns && this->nbLines == rhs.nbLines, 
    //             "Height and Width should already be the same if they are both Matrix4x4");

    return *this;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& matrix) const
{
    assert(this->matrix != nullptr && matrix.matrix != nullptr);

    Matrix4x4 resultMatrix;

    for (unsigned int i = 0; i < this->nbLines; i++)
    {
        for (unsigned int j = 0; j < this->nbColumns; j++)
        {
            float result = 0.f;
            for (unsigned int k = 0; k < this->nbColumns; k++)
            {
                result += this->matrix[i * nbColumns + k] 
                        * matrix.matrix[j + k * nbColumns];    
            }

            resultMatrix.matrix[j + i * nbColumns] = result;
        }
    }
    return resultMatrix;
}

Vec4 Matrix4x4::operator*(const Vec4& vect) const
{
    Vec4 resultVect(0,0,0,0);
    for (unsigned int i = 0; i < nbLines; i++)
    {
        for (unsigned int j = 0; j < nbColumns; j++)
            resultVect[i] += matrix[(i*nbColumns)+j]*vect[j];  
        
    }
    return resultVect;
}


Vec3 Matrix4x4::getXAxis() const 
{
    return Vec3{matrix[0], matrix[4], matrix[8]};
}

Vec3 Matrix4x4::getYAxis() const
{
    return Vec3{matrix[1], matrix[5], matrix[9]};
}

Vec3 Matrix4x4::getZAxis() const
{
    return Vec3{matrix[2], matrix[6], matrix[10]};
}

Vec3 Matrix4x4::getScale() const
{
    return Vec3{getXAxis().vectorLength(), getYAxis().vectorLength(), getZAxis().vectorLength()};
}

Vec3 Matrix4x4::getTranslationVector() const
{
    assert(matrix != nullptr);
    return Vec3(matrix[3], matrix[7], matrix[11]);
}

void Matrix4x4::normalizeScale()
{
    Core::Maths::Vec3 axis1 = getXAxis().unitVector();
    Core::Maths::Vec3 axis2 = getYAxis().unitVector();
    Core::Maths::Vec3 axis3 = getZAxis().unitVector();

    matrix[0] = axis1.x;
    matrix[4] = axis1.y;
    matrix[8] = axis1.z;

    matrix[1] = axis2.x;
    matrix[5] = axis2.y;
    matrix[9] = axis2.z;

    matrix[2]  = axis3.x;
    matrix[6]  = axis3.y;
    matrix[10] = axis3.z;
}

Matrix4x4 Matrix4x4::getInverse() const
{
    Matrix4x4 mInv;

    mInv.matrix[0] = matrix[5]  * matrix[10] * matrix[15] - 
                     matrix[5]  * matrix[11] * matrix[14] - 
                     matrix[9]  * matrix[6]  * matrix[15] + 
                     matrix[9]  * matrix[7]  * matrix[14] +
                     matrix[13] * matrix[6]  * matrix[11] - 
                     matrix[13] * matrix[7]  * matrix[10];

    mInv.matrix[4] = -matrix[4]  * matrix[10] * matrix[15] + 
                      matrix[4]  * matrix[11] * matrix[14] + 
                      matrix[8]  * matrix[6]  * matrix[15] - 
                      matrix[8]  * matrix[7]  * matrix[14] - 
                      matrix[12] * matrix[6]  * matrix[11] + 
                      matrix[12] * matrix[7]  * matrix[10];

    mInv.matrix[8] = matrix[4]  * matrix[9] * matrix[15] - 
                     matrix[4]  * matrix[11] * matrix[13] - 
                     matrix[8]  * matrix[5] * matrix[15] + 
                     matrix[8]  * matrix[7] * matrix[13] + 
                     matrix[12] * matrix[5] * matrix[11] - 
                     matrix[12] * matrix[7] * matrix[9];

    mInv.matrix[12] = -matrix[4]  * matrix[9] * matrix[14] + 
                       matrix[4]  * matrix[10] * matrix[13] +
                       matrix[8]  * matrix[5] * matrix[14] - 
                       matrix[8]  * matrix[6] * matrix[13] - 
                       matrix[12] * matrix[5] * matrix[10] + 
                       matrix[12] * matrix[6] * matrix[9];

    mInv.matrix[1] = -matrix[1]  * matrix[10] * matrix[15] + 
                      matrix[1]  * matrix[11] * matrix[14] + 
                      matrix[9]  * matrix[2] * matrix[15] - 
                      matrix[9]  * matrix[3] * matrix[14] - 
                      matrix[13] * matrix[2] * matrix[11] + 
                      matrix[13] * matrix[3] * matrix[10];

    mInv.matrix[5] = matrix[0]  * matrix[10] * matrix[15] - 
                     matrix[0]  * matrix[11] * matrix[14] - 
                     matrix[8]  * matrix[2] * matrix[15] + 
                     matrix[8]  * matrix[3] * matrix[14] + 
                     matrix[12] * matrix[2] * matrix[11] - 
                     matrix[12] * matrix[3] * matrix[10];

    mInv.matrix[9] = -matrix[0]  * matrix[9] * matrix[15] + 
                      matrix[0]  * matrix[11] * matrix[13] + 
                      matrix[8]  * matrix[1] * matrix[15] - 
                      matrix[8]  * matrix[3] * matrix[13] - 
                      matrix[12] * matrix[1] * matrix[11] + 
                      matrix[12] * matrix[3] * matrix[9];

    mInv.matrix[13] = matrix[0]  * matrix[9] * matrix[14] - 
                      matrix[0]  * matrix[10] * matrix[13] - 
                      matrix[8]  * matrix[1] * matrix[14] + 
                      matrix[8]  * matrix[2] * matrix[13] + 
                      matrix[12] * matrix[1] * matrix[10] - 
                      matrix[12] * matrix[2] * matrix[9];

    mInv.matrix[2] = matrix[1]  * matrix[6] * matrix[15] - 
                     matrix[1]  * matrix[7] * matrix[14] - 
                     matrix[5]  * matrix[2] * matrix[15] + 
                     matrix[5]  * matrix[3] * matrix[14] + 
                     matrix[13] * matrix[2] * matrix[7] - 
                     matrix[13] * matrix[3] * matrix[6];

    mInv.matrix[6] = -matrix[0]  * matrix[6] * matrix[15] + 
                      matrix[0]  * matrix[7] * matrix[14] + 
                      matrix[4]  * matrix[2] * matrix[15] - 
                      matrix[4]  * matrix[3] * matrix[14] - 
                      matrix[12] * matrix[2] * matrix[7] + 
                      matrix[12] * matrix[3] * matrix[6];

    mInv.matrix[10] = matrix[0]  * matrix[5] * matrix[15] - 
                      matrix[0]  * matrix[7] * matrix[13] - 
                      matrix[4]  * matrix[1] * matrix[15] + 
                      matrix[4]  * matrix[3] * matrix[13] + 
                      matrix[12] * matrix[1] * matrix[7] - 
                      matrix[12] * matrix[3] * matrix[5];

    mInv.matrix[14] = -matrix[0]  * matrix[5] * matrix[14] + 
                       matrix[0]  * matrix[6] * matrix[13] + 
                       matrix[4]  * matrix[1] * matrix[14] - 
                       matrix[4]  * matrix[2] * matrix[13] - 
                       matrix[12] * matrix[1] * matrix[6] + 
                       matrix[12] * matrix[2] * matrix[5];

    mInv.matrix[3] = -matrix[1] * matrix[6] * matrix[11] + 
                      matrix[1] * matrix[7] * matrix[10] + 
                      matrix[5] * matrix[2] * matrix[11] - 
                      matrix[5] * matrix[3] * matrix[10] - 
                      matrix[9] * matrix[2] * matrix[7] + 
                      matrix[9] * matrix[3] * matrix[6];

    mInv.matrix[7] = matrix[0] * matrix[6] * matrix[11] - 
                     matrix[0] * matrix[7] * matrix[10] - 
                     matrix[4] * matrix[2] * matrix[11] + 
                     matrix[4] * matrix[3] * matrix[10] + 
                     matrix[8] * matrix[2] * matrix[7] - 
                     matrix[8] * matrix[3] * matrix[6];

    mInv.matrix[11] = -matrix[0] * matrix[5] * matrix[11] + 
                       matrix[0] * matrix[7] * matrix[9] + 
                       matrix[4] * matrix[1] * matrix[11] - 
                       matrix[4] * matrix[3] * matrix[9] - 
                       matrix[8] * matrix[1] * matrix[7] + 
                       matrix[8] * matrix[3] * matrix[5];

    mInv.matrix[15] = matrix[0] * matrix[5] * matrix[10] - 
                      matrix[0] * matrix[6] * matrix[9] - 
                      matrix[4] * matrix[1] * matrix[10] + 
                      matrix[4] * matrix[2] * matrix[9] + 
                      matrix[8] * matrix[1] * matrix[6] - 
                      matrix[8] * matrix[2] * matrix[5];

    float det = matrix[0] * mInv.matrix[0] + matrix[1] * mInv.matrix[4] + matrix[2] * mInv.matrix[8] + matrix[3] * mInv.matrix[12];

    det = 1.0 / det;

    for (uint i = 0; i < 16; i++)
        mInv.matrix[i] = mInv.matrix[i] * det;

    return mInv;
}

Matrix4x4 Matrix4x4::CreateScaleMatrix(Vec3 scale)
{
    Matrix4x4 matrix;

    memset(matrix.matrix, 0, 4*4*sizeof(float));

    matrix.matrix[0]  = scale.x;
    matrix.matrix[5]  = scale.y;
    matrix.matrix[10] = scale.z;
    matrix.matrix[15] = 1;

    return matrix;
}

Matrix4x4 Matrix4x4::CreateTranslationMatrix(Vec3 translation)
{
    Matrix4x4 matrix;

    memset(matrix.matrix, 0, nbElements * sizeof(float));

    matrix.matrix[0] = 1;
    matrix.matrix[3] = translation.x;
    matrix.matrix[5] = 1;
    matrix.matrix[7] = translation.y;
    matrix.matrix[10] =1;
    matrix.matrix[11] =translation.z;
    matrix.matrix[15] =1;

    return matrix;
}

Matrix4x4 Matrix4x4::CreateXRotationMatrix(float angleX)
{
    Matrix4x4 matrix;
    float cosAngle = cos(angleX);
    float sinAngle = sin(angleX);

    matrix.matrix[0]  = 1;
    matrix.matrix[1]  = 0;
    matrix.matrix[2]  = 0;
    matrix.matrix[3]  = 0;

    matrix.matrix[4]  = 0;
    matrix.matrix[5]  = cosAngle;
    matrix.matrix[6]  = -sinAngle;
    matrix.matrix[7]  = 0;

    matrix.matrix[8]  = 0;
    matrix.matrix[9]  = sinAngle;
    matrix.matrix[10] = cosAngle;
    matrix.matrix[11] = 0;

    matrix.matrix[12] = 0;
    matrix.matrix[13] = 0;
    matrix.matrix[14] = 0;
    matrix.matrix[15] = 1;

    return matrix;
}

Matrix4x4 Matrix4x4::CreateYRotationMatrix(float angleY)
{
    Matrix4x4 matrix;
    float cosAngle = cos(angleY);
    float sinAngle = sin(angleY);

    matrix.matrix[0] = cosAngle;
    matrix.matrix[1] = 0;
    matrix.matrix[2] = sinAngle;
    matrix.matrix[3] = 0;

    matrix.matrix[4] = 0;
    matrix.matrix[5] = 1;
    matrix.matrix[6] = 0;
    matrix.matrix[7] = 0;

    matrix.matrix[8]  = -sinAngle;
    matrix.matrix[9]  = 0;
    matrix.matrix[10] = cosAngle;
    matrix.matrix[11]  = 0;

    matrix.matrix[12] = 0;
    matrix.matrix[13] = 0;
    matrix.matrix[14] = 0;
    matrix.matrix[15] = 1;

    return matrix;
}

Matrix4x4 Matrix4x4::CreateZRotationMatrix(float angleZ)
{
    Matrix4x4 matrix;
    float cosAngle = cos(angleZ);
    float sinAngle = sin(angleZ);
    
    matrix.matrix[0] = cosAngle;
    matrix.matrix[1] = -sinAngle;
    matrix.matrix[2] = 0;
    matrix.matrix[3] = 0;

    matrix.matrix[4] = sinAngle;
    matrix.matrix[5] = cosAngle;
    matrix.matrix[6] = 0;
    matrix.matrix[7] = 0;

    matrix.matrix[8]  = 0;
    matrix.matrix[9]  = 0;
    matrix.matrix[10] = 1;
    matrix.matrix[11] = 0;

    matrix.matrix[12] = 0;
    matrix.matrix[13] = 0;
    matrix.matrix[14] = 0;
    matrix.matrix[15] = 1;

    return matrix;
}


Matrix4x4 Matrix4x4::CreateFixedAngleEulerRotationMatrix(Vec3 angles)
{
    //convert Matrix to Matrix4x4 calling Matrix4x4::Matrix4x4 (const Matrix& copiedMatrix);
    //not opti
    return CreateYRotationMatrix(angles.y) * CreateXRotationMatrix(angles.x) * CreateZRotationMatrix(angles.z);
}


Matrix4x4 Matrix4x4::CreateTRSMatrix(Vec3 scale, Vec3 translation, Vec3 angles)
{
    return CreateTranslationMatrix(translation) 
            * CreateFixedAngleEulerRotationMatrix(angles) 
            * CreateScaleMatrix(scale);
}

Matrix4x4 Matrix4x4::CreateTRSMatrix(const Physics::Transform& transform)
{
    return CreateTranslationMatrix(transform.location) 
            * CreateFixedAngleEulerRotationMatrix(transform.rotation) 
            * CreateScaleMatrix(transform.scale);
}

Matrix4x4 Matrix4x4::CreateAxisRotationMatrix(const Vec3& axis, float angle)
{
    Matrix4x4 mat;

    float c = std::cos(angle);
    float s = std::sin(angle);

    mat[0][0] = pow(axis.x, 2) * (1 - c) + c;
    mat[0][1] = axis.x * axis.y * (1 - c) - axis.z * s;
    mat[0][2] = axis.x * axis.z * (1 - c) + axis.y * s;
    mat[0][3] = 0;

    mat[1][0] = axis.x * axis.y * (1 - c) + axis.z * s;
    mat[1][1] = pow(axis.y, 2) * (1 - c) + c;
    mat[1][2] = axis.y * axis.z * (1 - c) + axis.y * s;
    mat[1][3] = 0;

    mat[2][0] = axis.x * axis.z * (1 - c) + axis.y * s;
    mat[2][1] = axis.y * axis.z * (1 - c) + axis.x * s;
    mat[2][2] = pow(axis.z, 2) * (1 - c) + c;
    mat[2][3] = 0;

    mat[3][0] = 0;
    mat[3][1] = 0;
    mat[3][2] = 0;
    mat[3][3] = 1;

    return  mat;
}


std::ostream& Core::Maths::operator<<(std::ostream& stream, const Matrix4x4& matrix)
{
    stream << "\n";
    
    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            std::cout << matrix[i][j] << "  ";
        }
        std::cout << std::endl;
    }

    return stream;
}


Matrix4x4 Matrix4x4::getProjectionMatrixOnZAxis(const float d)
{
    Matrix4x4 mat;

    mat[0][0] = 1;
    mat[0][1] = 0;
    mat[0][2] = 0;
    mat[0][3] = 0;

    mat[1][0] = 0;
    mat[1][1] = 1;
    mat[1][2] = 0;
    mat[1][3] = 0;

    mat[2][0] = 0;
    mat[2][1] = 0;
    mat[2][2] = 1;
    mat[2][3] = 0;

    mat[3][0] = 0;
    mat[3][1] = 0;
    mat[3][2] = -d;
    mat[3][3] = 0;

    return  mat;
}

Matrix4x4 Matrix4x4::CreatePerspectiveProjectionMatrix(int width, int height,float near,float far,float fov)
// {
//     // float ymax = tanf(fov * M_PI / 180.f / 2.f);

//     Matrix4x4 m;

//     float tanHalffov = tanf(fov * M_PI / 180.f / 2.f);

//     m.matrix[0] = 1/(tanHalffov * width / height);
//     m.matrix[1] = 0.0;
//     m.matrix[2] = 0.0;
//     m.matrix[3] = 0.0;

//     m.matrix[4] = 0.0;
//     m.matrix[5] = 1 / tanHalffov;
//     m.matrix[6] = 0.0;
//     m.matrix[7] = 0.0;

//     m.matrix[8] = 0.f;
//     m.matrix[9] = 0.f;
//     m.matrix[10] = - (far + near) / (far - near);//0;
//     m.matrix[11] = -1;
                                
//     m.matrix[12] = 0.0;
//     m.matrix[13] = 0.0;
//     m.matrix[14] = - ((2) * far * near) / (far - near); //0;
//     m.matrix[15] = 0.0;

//     return m;
// }
{
    Matrix4x4 m = Matrix4x4::zero(4,4);
    
    //convert to radian
    fov = fov / 180.f * M_PI;

    m[0][0] = 1 / ((width / height) * std::tan(fov / 2));
    m[1][1] = 1 / (std::tan(fov / 2));
    m[2][2] = -((far + near) / (far - near));

    m[2][3] = -((2 * far * near) / (far - near));
    m[3][2] = -1;

    return m;
}

Core::Maths::Matrix4x4 lookAt(const Core::Maths::Vec3& eye, 
                              const Core::Maths::Vec3& center, 
                              const Core::Maths::Vec3& up)
{
    Core::Maths::Vec3 zAxis((center - eye).unitVector());
    const Core::Maths::Vec3 xAxis((Core::Maths::Vec3::crossProduct(zAxis, up)).unitVector());
    const Core::Maths::Vec3 yAxis(Core::Maths::Vec3::crossProduct(xAxis, zAxis));

    zAxis.negate();

    Core::Maths::Matrix4x4 returned = Core::Maths::Matrix4x4::identity(4);
    returned[0][0] = xAxis.x;
    returned[0][1] = xAxis.y;
    returned[0][2] = xAxis.z;
    returned[0][3] =-Core::Maths::Vec3::dotProduct(xAxis, eye);
    returned[1][0] = yAxis.x;
    returned[1][1] = yAxis.y;
    returned[1][2] = yAxis.z;
    returned[1][3] =-Core::Maths::Vec3::dotProduct(yAxis, eye);
    returned[2][0] = zAxis.x;
    returned[2][1] = zAxis.y;
    returned[2][2] = zAxis.z;
    returned[2][3] =-Core::Maths::Vec3::dotProduct(zAxis, eye);
    returned[3][0] = 0;
    returned[3][1] = 0;
    returned[3][2] = 0;
    returned[3][3] = 1;

    return returned;
}