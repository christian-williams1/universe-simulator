#version 330 core

layout (location=0) out vec4 fragCol;
layout (location=1) out vec4 brightColor;

in vec3 normal;
in vec3 color;
in vec3 lightDir;

void main()
{
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = diff*vec3(1.0f, 1.0f, 1.0f);
    vec4 lighting = vec4(diffuse+0.001f, 1.0f)*vec4(color, 1.0f); //vec4(lightDir * 0.5 + 0.5, 1.0);

    fragCol = lighting;

    float brightness = dot(fragCol.xyz, vec3(0.2126, 0.7152, 0.0722));
    
    if (brightness > 1.0)
    {
        brightColor = fragCol;
    }
    else
    {
        brightColor = vec4(0.0f);
    }
}