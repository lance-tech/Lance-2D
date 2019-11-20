#version 330 core

in vec3 fragmentColor;
in vec2 UV;

uniform int rowCount;
uniform int columnCount;

uniform vec4 inputColor;

out vec4 color;

void main()
{
	int stepCountRow = rowCount;
	int stepCountColumn = columnCount;
	float lineWidth = 0.9;
	float lineOffset = (1-lineWidth) * 0.5;
	float stripeX = floor(fract((UV.x * stepCountColumn) + lineOffset) + lineWidth);
	float stripeY = floor(fract((UV.y * stepCountRow) + lineOffset) + lineWidth);
	
	float gridBoard = stripeX * stripeY;

//	float stripeY = floor(fract(UV.y * stepCountRow) + 0.5);

	float inv_stripeX = 1 - stripeX;
	float inv_stripeY = 1 - stripeY;

	float check1 = stripeX + stripeY;
	float check2 = inv_stripeX + inv_stripeY;

	float checkBoard = check1 * check2;
	

//	float gradientColor = pow( UV.x, 2.0f );
//	color = gradientColor * vec3(1, 1, 1);
//	color.rgb = inputColor.rgb * ((gridBoard) * fragmentColor);
	color.rgb = inputColor.rgb * ((gridBoard + 0.9) * fragmentColor)*0.2;
	color.a = inputColor.a;

//	color = (1-pow(1-UV.x, 20)) * (1 - pow(UV.x, 20)) *  fragmentColor;
//	color = texture( mySampler, UV ).rgb;
}
