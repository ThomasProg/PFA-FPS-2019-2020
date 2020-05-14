#version 330 core

layout (location = 0) in vec3 v_loc;
layout (location = 1) in vec2 v_uvs;
layout (location = 2) in vec3 v_normals;

out vec4 f_color;
out vec2 f_uvs;
out vec3 f_normals;

out vec3 f_location;

uniform mat4 mvp;

uniform mat4 camera;
uniform mat4 model;

void main()
{
    gl_Position = mvp * vec4(v_loc, 1.0);

    f_location  = (model * vec4(v_loc, 1.0)).xyz;  

    f_uvs = v_uvs;
    f_normals = (model * vec4(normalize(v_normals), 0)).xyz;
}