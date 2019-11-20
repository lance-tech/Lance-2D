#version 330 core

in vec3 fragmentColor;
in vec2 UV;

out vec4 color;
uniform vec4 inputColor;
uniform sampler2D TextureSampler0;


void main()
{
	vec3 textureColor = texture(TextureSampler0, UV).rgb;

	color = vec4(textureColor * fragmentColor * inputColor.rgb, inputColor.a);
}
