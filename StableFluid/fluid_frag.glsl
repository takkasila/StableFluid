#version 420 core

uniform vec2 screenSize;
uniform samplerBuffer u_densityFloat_tbo_tex;

out vec4 color;

void main()
{
    int pos = int(gl_FragCoord.x + gl_FragCoord.y*screenSize.x);
    color = vec4(texelFetch(u_densityFloat_tbo_tex, pos).r);
	color.a = 1.0;
}
