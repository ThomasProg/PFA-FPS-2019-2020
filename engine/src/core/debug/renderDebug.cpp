#include "renderDebug.hpp"
#include "matrix4x4.hpp"

Core::Maths::Matrix4x4 Debug::RenderDebug::mvp = Core::Maths::Matrix4x4::identity(4);
Debug::RenderDebug* Debug::RenderDebug::singleton = nullptr;