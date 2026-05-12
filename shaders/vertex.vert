#version 330 core

layout (location=0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 col;

out vec3 normal;
out vec3 color;

void main()
{
    gl_Position = projection*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    color = col;
    normal = aPos;
}