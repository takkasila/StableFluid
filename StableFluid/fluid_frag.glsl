#version 420 core

uniform ivec2 screenSize;
uniform ivec2 cellCount;
uniform samplerBuffer u_densityFloat_tbo_tex;

out vec4 color;

int AT(int x, int y)
{
    return x+y*(cellCount.x+2);
}

float lerp(float a, float b, float amount)
{
    return a+(b-1)*clamp(amount, 0, 1);
}

void main()
{
    vec2 cellSize = screenSize / cellCount;
    vec2 cellIndex = floor(gl_FragCoord.xy/cellSize);
    //Four closest neighbor
    vec2 p1 = cellSize * (cellIndex + (0.5, 0.5));
    vec2 p2 = cellSize * (cellIndex + (1.5, 0.5));
    vec2 p3 = cellSize * (cellIndex + (1.5, 1.5));
    vec2 p4 = cellSize * (cellIndex + (0.5, 1.5));

    //Getting shade
    int x, y;
    x = int(cellIndex.x)+1;
    y = int(cellIndex.y)+1;
    float shadeP1 = texelFetch(u_densityFloat_tbo_tex, AT(x,y)).r;
    float shadeP2 = texelFetch(u_densityFloat_tbo_tex, AT(x+1,y)).r;
    float shadeP3 = texelFetch(u_densityFloat_tbo_tex, AT(x+1,y+1)).r;
    float shadeP4 = texelFetch(u_densityFloat_tbo_tex, AT(x,y+1)).r;

    //Lerp
    vec2 lerpAmount = (gl_FragCoord.xy - p1) / (p3-p1);
    float shade = lerp(
        lerp(shadeP1, shadeP2, lerpAmount.x) //Bottom
        , lerp(shadeP4, shadeP3, lerpAmount.x) //Top
        , lerpAmount.y);

	color = vec4(vec3(shade), 1);
}
