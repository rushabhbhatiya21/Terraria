#version 330 core

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D uTexture;

out vec4 finalColor;

void main()
{
    vec4 textureColor = texture(uTexture, fragTexCoord);
    finalColor = textureColor * fragColor;
}