#version 330 core

in vec2 uv;
in vec4 tint;

uniform sampler2D uTexture;

out vec4 fragColor;

void main()
{
    vec4 textureColor = texture(uTexture, uv);
    fragColor = textureColor * tint;
}