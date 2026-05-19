#version 330 core

layout (location=0) out vec4 fragCol;
layout (location=1) out vec4 brightColor;

void main()
{
    vec2 cxy = 2.0 * gl_PointCoord - 1.0;
    
    // Discard if outside the unit circle
    if (dot(cxy, cxy) > 1.0) {
        discard;
    }

    fragCol = vec4(1.0f);
    brightColor = vec4(0.0f);
}