#version 330 core

in vec3 fragmentColor;
in vec2 UV;

out vec4 color;

uniform vec4 inputColor;
uniform vec2 scaleFactor;
uniform float specularFactor;

void main()
{
	float bright = 0.35;
	float embosWidth = 0.15;
	float embosWidth2 = 1.0 - embosWidth;
	float borderWidth = 0.95;

	float embosX = floor(fract(1-UV.x) + (embosWidth - (0.004 * scaleFactor.x))) * bright;
	float embosY = floor(fract(UV.y) + (embosWidth - (0.004 * scaleFactor.y))) * bright;
	float embosX2 = floor(fract(1-UV.x) + (embosWidth2 + (0.004 * scaleFactor.x)));
	float embosY2 = floor(fract(UV.y) + (embosWidth2 + (0.004 * scaleFactor.y)));

	float embos = embosX + embosY;
	float embos2Inv = (embosX2 * embosY2);
	float embos2 = (embosX2 * bright) + (embosY2 * bright);

	float borderX = floor((fract(1-UV.x) * fract(UV.x)) + (borderWidth + (0.001 * scaleFactor.x )) );
	float borderY = floor((fract(UV.y) * fract(1-UV.y)) + (borderWidth + (0.001 * scaleFactor.y )) );
	float border = borderX * borderY;

	float spec = fract(1-UV.x) * fract(UV.y);


	color.rgb = (embos + ((inputColor.rgb + ((pow(spec, 3) * 2.5) * specularFactor)) * embos2)) * (border + 0.15);
	color.a = inputColor.a;
}
