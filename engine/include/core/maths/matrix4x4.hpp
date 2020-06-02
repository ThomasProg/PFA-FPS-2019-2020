#ifndef _MATRIX_4x4_HPP_
#define _MATRIX_4x4_HPP_

#include <array>
#include "matrix.hpp"

namespace Physics
{
    struct Transform;
}

namespace Core::Maths
{
    struct Vec3;
    struct Vec4;

    class Matrix4x4 final : public Matrix
    {
    private:
        static constexpr unsigned int nbElements = 16;

    public:
        Matrix4x4();
        Matrix4x4(const Matrix4x4& copiedMatrix);
        Matrix4x4(Matrix&&);
        ~Matrix4x4() = default;

        Matrix4x4& operator=(const Matrix4x4& rhs);
        Matrix4x4& operator=(Matrix4x4&& rhs);
        Vec4 operator*(const Vec4&) const;
        inline Matrix4x4 operator*(const Matrix4x4&) const;
        inline static void multiply(const Matrix4x4& lhs, const Matrix4x4& rhs, Matrix4x4& resultBuffer);

        Vec3 getXAxis() const;
        Vec3 getYAxis() const;
        Vec3 getZAxis() const;

        Vec3 getScale() const;
        Vec3 getTranslationVector() const;

        void normalizeScale();
        Matrix4x4 getInverse() const;

        static Matrix4x4 getProjectionMatrixOnZAxis(const float d);
        static Matrix4x4 CreatePerspectiveProjectionMatrix(int width, int height,float near,float far,float fov);
        static Core::Maths::Matrix4x4 lookAt(const Core::Maths::Vec3& eye, const Core::Maths::Vec3& center, const Core::Maths::Vec3& up);

        static Matrix4x4 CreateScaleMatrix(Vec3 scale);
        static Matrix4x4 CreateTranslationMatrix(Vec3 translation);

        static Matrix4x4 CreateXRotationMatrix(float angleX);
        static Matrix4x4 CreateYRotationMatrix(float angleY);
        static Matrix4x4 CreateZRotationMatrix(float angleZ);

        static Matrix4x4 CreateFixedAngleEulerRotationMatrix(Vec3 angles);

        static Matrix4x4 CreateTRSMatrix(Vec3 scale, Vec3 translation, Vec3 angles);
        static Matrix4x4 CreateTRSMatrix(const Physics::Transform& transform);

        static Matrix4x4 CreateAxisRotationMatrix(const Vec3& axis, float angle); 
    };

    std::ostream& operator<<(std::ostream& stream, const Matrix4x4& vector);
}

#include "matrix4x4.inl"

#endif