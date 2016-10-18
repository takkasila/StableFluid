#version 420 core

uniform vec2 screenSize;

out vec4 color;

void main()
{
    color = vec4(gl_FragCoord.xy/screenSize, 0, 1);
}
