#include "vec2.hpp"

inline Core::Maths::Vec2::Vec2(const Core::Maths::Vec2& f32) noexcept
{
    this->x = f32.x;
    this->y = f32.y;
}

inline Core::Maths::Vec2::Vec2(float f) noexcept
{
    this->x = f;
    this->y = f;
}

inline Core::Maths::Vec2::Vec2(float x, float y) noexcept
{
    this->x = x;
    this->y = y;
}