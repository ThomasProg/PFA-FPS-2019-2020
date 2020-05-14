#ifndef _RENDER_DEBUG_HPP_
#define _RENDER_DEBUG_HPP_

#ifndef NDEBUG

#include "shader.hpp"

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "matrix4x4.hpp"

namespace Debug
{
    class RenderDebug
    {
    private:
        unsigned int VBO, VAO;
        Resources::Shader shader{"resources/shaders/flatColor.vert", "resources/shaders/flatColor.frag"};

        static RenderDebug* singleton;

    public:

        static Core::Maths::Matrix4x4 mvp;

        float vertices[6] = {
            0, 2, 0,
            1, 1, 1,
        };

        static RenderDebug* getSingleton()
        {
            if (singleton == nullptr)
            {
                singleton = new RenderDebug();
                singleton->init();
            }
            return singleton;
        }

        void init()
        {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
            glEnableVertexAttribArray(0);

            // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0); 

            // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
            // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
            glBindVertexArray(0); 
        }

        void draw()
        {
            shader.use();
            glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            // update
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            
            GLuint mvpID = shader.getUniformLocation("mvp");

            glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

            GLuint colorID = shader.getUniformLocation("color");

            float color[4] = {1.f,0,0,1.f};
            glUniform4fv(colorID, 1, &color[0]);

            glDrawArrays(GL_LINES, 0, 2);
        }
    };

}

#endif

#endif