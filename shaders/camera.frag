#version 330 core
out vec4 fragCol;

uniform vec3 col;

void main()
{
    fragCol = vec4(col, 1.0);
}