#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include "model.hpp"

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <string>
#include <vector>

namespace Renderer
{
    struct LightData;

    struct Mesh;
    struct Camera;
    class LightManager;
}

namespace Resources 
{
    class Shader
    {
    protected:
        // OpenGL shader program Index on GPU
        GLuint programID = 0;

    public:
        Shader() = default;
        Shader(const char* vsFilename, const char* fsFilename);

        Shader(const Shader&) = delete;
        Shader(Shader&&);
        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&);

        ~Shader();

        virtual void loadUniformValuesLocation() {}
        virtual void useUniformValues(const Renderer::Camera& cam, const Renderer::Mesh& mesh) const {}
        virtual void useLightsUniformValues(const Renderer::LightManager&) const {}

        // Loads vertex from file.
        // Filename is the file's name.
        // ShaderType is the type the shader should be interpreted.
        // outID is the returned shader ID.
        // If the shader is loaded with success, the return value is true. 
        // Else, it is false.
        static bool loadShader(const std::string& filename, GLuint shaderType, GLuint& outID);
        // Loads vertex shader from file.
        // Filename is the file's name.
        // outID is the returned shader ID.
        // If the shader is loaded with success, the return value is true. 
        // Else, it is false.
        static bool loadVertexShader(const std::string& filename, GLuint& outID);
        // Loads fragment shader from file.
        // Filename is the file's name.
        // outID is the returned shader ID.
        // If the shader is loaded with success, the return value is true. 
        // Else, it is false.
        static bool loadFragmentShader(const std::string& filename, GLuint& outID);
        static GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);

        // Sets glVertexAttribPointer to corresponding program. 
        void setAttributePointer(GLuint programID, GLuint attributeVBOID, const char* attributeName, unsigned int nbElemsPerStride = 3) const;

        // Links vbos to program.
        void linkModel(const Resources::Model&) const;

        // // Links texture buffer to program. 
        // void linkTexture(const Texture<>&, const GLuint VAO) const;

        inline constexpr bool isValid() const noexcept;

        inline void use() const;

        inline GLuint getUniformLocation(const char* str) const;
    };
}

#include "shader.inl"

#endif