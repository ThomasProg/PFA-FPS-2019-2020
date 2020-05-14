#version 330 core

layout (location = 0) in vec3 v_loc;

uniform mat4 mvp;

uniform vec4 color;

void main()
{
    gl_Position = mvp * vec4(v_loc, 1.0);
}