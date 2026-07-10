#version 330 core

in vec2 vUV;
in vec4 vTint;
uniform sampler2D uTexture;

out vec4 fragColor;

void main()
{
	vec4 textureColor = texture(uTexture, vUV);
	fragColor = textureColor * vTint;
}