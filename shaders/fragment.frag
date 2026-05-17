#version 330 core

out vec4 fragCol;

in vec3 normal;
in vec3 color;
in vec3 lightDir;

void main()
{
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff*vec3(1.0f, 1.0f, 1.0f);
    fragCol = vec4(diffuse+0.2f, 1.0f)*vec4(color, 1.0f); //vec4(lightDir * 0.5 + 0.5, 1.0);
}