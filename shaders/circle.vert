#version 300 es
precision highp float;
layout (location = 0) in lowp vec3 aPos;
layout (location = 1) in lowp vec4 pPos;
layout (location = 2) in lowp vec4 pCol;
uniform mat4 transform;
out vec4 coord;
out vec4 col;
void main()
{
	//vec4 tCoords = vec4(aPos, 1.0) * transform;
	//tCoords *= pPos.w;
	vec4 pPost = (pPos + (vec4(aPos, 1.0) * pPos.w)) * transform;
	// pPost += 2.0;
	// pPos *= transform;
    gl_Position = vec4(pPost.x, pPost.y, 1.0, 1.0);
	coord = vec4(aPos, 1.0);
	col = pCol;
}