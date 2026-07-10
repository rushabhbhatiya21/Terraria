#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec4 aTint;

out vec2 uv;
out vec4 tint;

uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * vec4(aPosition, 0.0, 1.0);
	uv = aUV;
	tint = aTint;
}
