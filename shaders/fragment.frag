#version 330 core

out vec4 fragCol;

in vec3 normal;
in vec3 color;

void main()
{
    vec3 lightDir = normalize(vec3(1.0f, 0.0f, 0.0f));
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff*vec3(1.0f, 1.0f, 1.0f);
    fragCol = vec4(diffuse, 1.0f)*vec4(color, 1.0f);
}