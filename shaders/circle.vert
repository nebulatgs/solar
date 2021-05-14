#version 300 es
precision lowp float;
layout (location = 0) in lowp vec3 aPos;
// layout (location = 1) in lowp vec4 pPos;
// layout (location = 2) in lowp vec4 pCol;
uniform mat4 transform;
out vec4 coord;
out vec4 col;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0) * transform;
	coord = vec4(aPos, 1.0);
	col = vec4(aPos, 1.0);
}