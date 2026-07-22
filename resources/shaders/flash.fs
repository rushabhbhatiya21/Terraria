#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D uTexture;
uniform float flash;

out vec4 finalColor;

void main()
{
	vec4 texel = texture(uTexture, fragTexCoord);

	vec3 color = mix(texel.rgb, vec3(1.0), flash);

	finalColor = vec4(color, texel.a);
}