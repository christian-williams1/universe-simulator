#version 330 core

out vec4 fragCol;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    fragCol = texture(skybox, TexCoords);
}