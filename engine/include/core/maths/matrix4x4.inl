#include "matrix4x4.hpp"

#include <cassert>

inline Core::Maths::Matrix4x4 Core::Maths::Matrix4x4::operator*(const Core::Maths::Matrix4x4& matrix) const
{
    assert(this->matrix != nullptr && matrix.matrix != nullptr);

    Core::Maths::Matrix4x4 resultMatrix;
    float result;
    for (unsigned int i = 0; i < this->nbLines; i++)
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