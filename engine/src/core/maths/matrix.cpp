#include <assert.h>
#include <string.h>
#include <iostream>
#include <cmath>

#include "matrix.hpp"

using namespace Core::Maths;

Matrix::Matrix(unsigned int nbLines, unsigned int nbColumns)
{
    this->nbLines = nbLines;
    this->nbColumns = nbColumns;

    this->matrix = new float[nbLines * nbColumns];
}

Matrix::Matrix(const Matrix& copiedMatrix)
{
    this->nbColumns = copiedMatrix.nbColumns;
    this->nbLines   = copiedMatrix.nbLines;
    this->matrix    = new float[this->nbLines * this->nbColumns];
    
    memcpy(this->matrix, copiedMatrix.matrix, sizeof(float) * this->nbColumns * this->nbLines);
}

Matrix::Matrix(Matrix&& movedMatrix)
{
    this->nbColumns = movedMatrix.nbColumns;
    this->nbLines   = movedMatrix.nbLines;
        
    matrix = movedMatrix.matrix;
    movedMatrix.matrix = nullptr;
}

Matrix::Matrix(const Matrix* copiedMatrix)
{
    this->nbColumns = copiedMatrix->nbColumns;
    this->nbLines   = copiedMatrix->nbLines;
    this->matrix    = new float[this->nbLines * this->nbColumns];

    // float* matrixArray = copiedMatrix->matrix; //for opti 
    
    memcpy(this->matrix, copiedMatrix->matrix, sizeof(float) * this->nbColumns * this->nbLines);
}

Matrix::~Matrix()
{
    if (matrix != nullptr)
        delete[] matrix;
}

Matrix Matrix::identity(const unsigned int size)
{
    Matrix matrix(size, size);

    unsigned int totalSize = size * size;

    for (unsigned int i = 0; i < totalSize; i++)
    {
        matrix.matrix[i] = i % (size + 1) == 0 ? 1 : 0;
    }

    return matrix;
}

Matrix Matrix::zero(const unsigned int nbLines, const unsigned int nbColumns)
{
    Matrix zeroMatrix;

    zeroMatrix.nbColumns = nbColumns;
    zeroMatrix.nbLines   = nbLines;
    zeroMatrix.matrix    = new float[nbLines * nbColumns];
    
    memset(zeroMatrix.matrix, 0, sizeof(float) * nbColumns * nbLines);

    return zeroMatrix;
}

float Matrix::accessor(const unsigned int line, const unsigned int column)
{
    // LOGIC_ASSERT(line < nbLines && column < nbColumns, "Invalid arguments : column or line out of bounds.");
    return this->matrix[column + line * this->nbColumns];
}

Matrix Matrix::transpose() const
{
    Matrix transposedMatrix(this->nbColumns, this->nbLines);

    for (unsigned int i = 0; i < nbLines; i++)
    {
        for (unsigned int j = 0; j < nbColumns; j++)
        {
            transposedMatrix.matrix[i + j * nbLines] = this->matrix[j + i * nbColumns];    
        }
    }

    return transposedMatrix;
}

Matrix& Matrix::transposeThis()
{
    for (unsigned int i = 0; i < nbLines; i++)
    {
        for (unsigned int j = i; j < nbColumns; j++)
        {
            std::swap(this->matrix[i + j * nbLines], this->matrix[j + i * nbColumns]);    
        }
    }

    return *this;
}

bool Matrix::operator==(const Matrix& lhs) const
{
    unsigned int index = 0;
    while (index < nbLines * nbColumns)
    {
        if (this->matrix[index] == lhs.matrix[index])
            index++;
        else 
            return false;
    }
    return true;
}

Matrix& Matrix::operator=(const Matrix& copiedMatrix)
{
    this->nbColumns = copiedMatrix.nbColumns;
    this->nbLines   = copiedMatrix.nbLines;
    if (this->matrix != nullptr)
        delete[] this->matrix;

    this->matrix    = new float[this->nbLines * this->nbColumns];
    
    memcpy(this->matrix, copiedMatrix.matrix, sizeof(float) * this->nbColumns * this->nbLines);

    return *this;
}

Matrix& Matrix::operator=(Matrix&& rhs)
{
    if (this->matrix != nullptr)
        delete[] this->matrix;
        
    this->matrix    = rhs.matrix;
    this->nbLines   = rhs.nbLines;
    this->nbColumns = rhs.nbColumns;
    rhs.matrix      = nullptr;
    return *this;
}

Matrix Matrix::operator+(const Matrix& matrix)
{
    assert(this->nbLines == matrix.nbLines && this->nbColumns == matrix.nbColumns);
    Matrix resultMatrix(this->nbLines, this->nbColumns);

    unsigned int size = nbLines * nbColumns;

    for (unsigned int i = 0; i < size; i++)
        resultMatrix.matrix[i] = this->matrix[i] - matrix.matrix[i];   

    return resultMatrix;
}

Matrix Matrix::operator-(const Matrix& matrix)
{
    // LOGIC_ASSERT(this->nbLines == matrix.nbLines && this->nbColumns == matrix.nbColumns, 
    //             "Can't substract matrices with different size");
    Matrix resultMatrix(this->nbLines, this->nbColumns);

    unsigned int size = nbLines * nbColumns;

    for (unsigned int i = 0; i < size; i++)
        resultMatrix.matrix[i] = this->matrix[i] - matrix.matrix[i];   

    return resultMatrix;
}

Matrix Matrix::operator*(const Matrix& matrix) const
{
    assert(this->nbColumns == matrix.nbLines); //required for multiplication
    Matrix resultMatrix(matrix.nbLines, this->nbColumns);

    float result;

    for (unsigned int i = 0; i < matrix.nbLines; i++)
    {
        for (unsigned int j = 0; j < this->nbColumns; j++)
        {
            result = 0.f;
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

Matrix Matrix::operator/(float f) const
{
    Matrix m(nbLines, nbColumns);

    for (unsigned int i = 0; i < nbLines * nbColumns; i++)
        m.matrix[i] = matrix[i] / f;
    
    return m;
}

float* Matrix::operator[](unsigned int x) const
{
    return &this->matrix[x * this->nbColumns];
}

float* Matrix::operator[](unsigned int x)
{
    return &this->matrix[x * this->nbColumns];
}

void Matrix::addElement(int x, int y, float element)
{
    this->matrix[x + y * this->nbColumns] = element;
}

Matrix Matrix::GetSubMatrix(const Matrix& matrix, unsigned int excludedColumnIndex, unsigned int excludedLineIndex)
{
    Matrix m(matrix.nbLines - 1, matrix.nbColumns - 1);
    unsigned int x = 0, y = 0;
    for (unsigned int iy = 0; iy < matrix.nbLines; iy++)
    {
        if (iy != excludedLineIndex)
        {
            for (unsigned int jx = 0; jx < matrix.nbLines; jx++)
            {
                if (jx != excludedColumnIndex)
                {
                    m[y][x] = matrix[iy][jx];
                    x++;
                }
            }   
            y++;
            x = 0;
        }
    }

    return m;
}

float Matrix::getDeterminant() const
{
    // LOGIC_ASSERT(nbLines == nbColumns, 
    //             "Can't get determinant of non squared matrix.");
    if (nbLines == 1)
        return matrix[0];
    else if (nbLines == 2)
        return matrix[0] * matrix[3] - matrix[1] * matrix[2];
    else if (nbLines == 3)
        return matrix[0] * (matrix[4] * matrix[8] - matrix[5] * matrix[7])
             - matrix[1] * (matrix[3] * matrix[8] - matrix[5] * matrix[6])
             + matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
    else if (nbLines == 4)
        return matrix[0] 
                * (matrix[5] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) 
                   - matrix[6] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) 
                   + matrix[7] * (matrix[9] * matrix[14] - matrix[10] * matrix[13]))
            -  matrix[1] 
                * (matrix[4] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) 
                  - matrix[6] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) 
                  + matrix[7] * (matrix[8] * matrix[14] - matrix[10] * matrix[12]))
            +  matrix[2] 
                * (matrix[4] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) 
                  - matrix[5] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) 
                  + matrix[7] * (matrix[8] * matrix[13] - matrix[9] * matrix[12]))
            -  matrix[3] 
                * (matrix[4] * (matrix[9] * matrix[14] - matrix[10] * matrix[13]) 
                  - matrix[5] * (matrix[8] * matrix[14] - matrix[10] * matrix[12]) 
                  + matrix[6] * (matrix[8] * matrix[13] - matrix[9] * matrix[12]));
    
    else 
    {
        float det = 0.f;
        for (unsigned int x = 0; x < this->nbLines; x++)
        {
            Matrix subMatrix = GetSubMatrix(*this, x, 0);
            det += (*this)[0][x] * subMatrix.getDeterminant() * std::pow(-1, x);
        }
        return det; 
    }
}

bool Matrix::isOrthogonal() const
{
    if (nbLines == nbColumns)
        return ((*this) * this->transpose()) == identity(nbLines);
    else 
        return false;
}

float Matrix::getMinorant(unsigned int excludedColumnIndex, unsigned int excludedLineIndex) const
{
    Matrix sub = GetSubMatrix(*this, excludedColumnIndex, excludedLineIndex);
    
    return sub.getDeterminant();
}

float Matrix::getCofactor(unsigned int column, unsigned int line) const
{
    return  std::pow((-1), column + line) * getMinorant(column, line);
}

Matrix Matrix::getComatrix() const
{
    Matrix m(nbLines, nbColumns);

    for (unsigned int y = 0; y < nbLines; y++)
    {
        for (unsigned int x = 0; x < nbColumns; x++)
        {
            m.matrix[x + y * nbColumns] = getCofactor(x, y);
        }
    }

    return m;
}

Matrix Matrix::getAdjoint() const
{
    return getComatrix().transpose();
}

Matrix Matrix::getInverse() const
{
    return getAdjoint() / getDeterminant();
}

// this + unknown = result
Matrix Matrix::resolveEquation(const Matrix& result) const
{
    Core::Maths::Matrix m = getInverse();
    return result * m;
}

std::ostream& Core::Maths::operator<<(std::ostream& stream, const Matrix& matrix)
{
    for (unsigned int i = 0; i < matrix.nbLines; i++)
    {
        for (unsigned int j = 0; j < matrix.nbColumns; j++)
        {
            std::cout << matrix.matrix[j + i * matrix.nbColumns] << "   ";
        }
        std::cout << std::endl;
    }
    return stream;
}