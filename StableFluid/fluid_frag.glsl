#version 420 core

uniform ivec2 screenSize;
uniform ivec2 cellCount;
uniform samplerBuffer u_densityFloat_tbo_tex;

out vec4 color;

void main()
{
    vec2 cellSize = screenSize / cellCount;
    vec2 cellPos = ceil( floor(gl_FragCoord.xy)/cellSize );
    int pos = int(cellPos.x) + int(cellPos.y)*cellCount.x;
	color = vec4(vec3(texelFetch(u_densityFloat_tbo_tex, pos).r), 1);
}
