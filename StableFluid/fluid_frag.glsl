#version 420 core

uniform vec2 screenSize;
uniform samplerBuffer u_densityFloat_tbo_tex;

out vec4 color;

void main()
{
    int pos = int(gl_FragCoord.x) + int(gl_FragCoord.y)*int(screenSize.x);
    float wtf = texelFetch(u_densityFloat_tbo_tex, pos).r;
	color = vec4(vec3(wtf), 1);
}
