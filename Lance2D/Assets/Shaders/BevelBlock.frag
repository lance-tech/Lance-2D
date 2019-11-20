#version 330 core

in vec3 fragmentColor;
in vec2 UV;

out vec4 color;

uniform vec4 inputColor;
uniform vec2 scaleFactor;
uniform float specularFactor;

void main()
{
	vec3 sourceColor = vec3(0.5, 0.5, 0.5) + (1 - (pow(UV.x * UV.y, .1) * 1.2));

	float tri1 = step(UV.y, UV.x);
	float tri2 = step(UV.x, 1-UV.y);

	float top = tri1 * tri2;
	float bottom = tri1 + tri2;
	float right = 1-(tri1 - tri2) * 0.8;
	float left = 1-(tri2 - tri1) * 0.8;

	float pirimid = top + (bottom * right * left);

	float edgeWidth = 0.025;
	float rect1 = step(edgeWidth, mod(UV.x * (0.5 + edgeWidth), 0.5));
	float rect2 = step(edgeWidth, mod(UV.y * (0.5 + edgeWidth), 0.5));
	float box = rect1 * rect2;

	float spec = pow(UV.x * UV.y, .1) * 0.5 * specularFactor;

	float edge = pirimid * (1-box);
	float bevelBox = edge + ((spec + box) * 0.75);

	vec3 finalColor = sourceColor * bevelBox;
	

    color = vec4(inputColor.rgb * finalColor, inputColor.a);
}
