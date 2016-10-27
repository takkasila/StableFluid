#version 420 core

uniform vec2 screenSize;
uniform samplerBuffer u_densityFloat_tbo_tex;

out vec4 color;

void main()
{
    int pos = int(gl_FragCoord.x) + int(gl_FragCoord.y)*int(screenSize.x);
	color = vec4(vec3(texelFetch(u_densityFloat_tbo_tex, pos).r), 1);
}
