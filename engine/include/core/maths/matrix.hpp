#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <array>
#include <iostream>

namespace Core::Maths
{
    class Matrix
    {
    public:
        float* matrix = nullptr;
        unsigned int nbLines, nbColumns;    

    public:
        Matrix() = default;
        Matrix(unsigned int nbLines, unsigned int nbColumns);
        Matrix(const Matrix&);
        Matrix(Matrix&&);
        Matrix(const Matrix*);
        ~Matrix();

        static Matrix identity(const unsigned int size);
        static Matrix zero(const unsigned int nbLines, const unsigned int nbColumns);

        float accessor(const unsigned int line, const unsigned int column);

        Matrix  transpose() const;
        Matrix& transposeThis();
        Matrix& operator=(const Matrix& copiedMatrix);
        Matrix& operator=(Matrix&& rhs);
        Matrix  operator+(const Matrix& matrix);
        Matrix  operator-(const Matrix& matrix);
        Matrix  operator*(const Matrix& matrix) const;
        Matrix  operator/(float f) const;
        float*  operator[](unsigned int id) const;
        float*  operator[](unsigned int id);

        Matrix operator/=(float f);

        bool operator==(const Matrix& lhs) const;

        void addElement(int x, int y, float element);

        static Matrix GetSubMatrix(const Matrix& matrix, unsigned int excludedColumnIndex, unsigned int excludedLineIndex);
        float getDeterminant() const;

        bool   isOrthogonal() const;
        float  getMinorant(unsigned int excludedColumnIndex, unsigned int excludedLineIndex) const;
        float  getCofactor(unsigned int column, unsigned int line) const;
        Matrix getComatrix() const;
        Matrix getAdjoint() const;
        Matrix getInverse() const;
        Matrix resolveEquation(const Matrix& result) const;
    };

    std::ostream& operator<<(std::ostream& stream, const Matrix& matrix);
}

#endif