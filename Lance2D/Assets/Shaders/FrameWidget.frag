#version 330 core

in vec3 fragmentColor;
in vec2 UV;

out vec4 color;

uniform vec3 inputColor;

void main()
{
//	int stepCount = 1;
//	float stripeX = floor(fract(UV.x * stepCount) + 0.5);
//	float stripeY = floor(fract(UV.y * stepCount) + 0.5);
//
//	float inv_stripeX = 1 - stripeX;
//	float inv_stripeY = 1 - stripeY;
//
//	float check1 = stripeX + stripeY;
//	float check2 = inv_stripeX + inv_stripeY;
//
//	float checkBoard = check1 * check2;

//	float embosX = (1 - pow(1-UV.x, 5)) * (1 - pow(UV.x, 5));
//	float embosY = (1 - pow(1-UV.y, 5)) * (1 - pow(UV.y, 5));

//	float embosX =(pow(1-UV.x, 5)) + (1 - pow(UV.x, 5));
//	float embosY =(pow(UV.y, 5)) + (1 - pow(UV.y, 5));


	float embosX = floor(fract(1-UV.x) + 0.1) * 0.25;
	float embosY = floor(fract(UV.y) + 0.1) * 0.25;

	float embosX2 =(1 - pow(UV.x, 15));
	float embosY2 =(1 - pow(1-UV.y, 15));

	float embos = embosX + embosY;
	float embos2 = embosX2 * embosY2;

//	float gradientColor = pow( UV.x, 2.0f );
//	color = gradientColor * vec3(1, 1, 1);
//	color = checkBoard * fragmentColor;
//	color = UV.x * checkBoard * fragmentColor;
//	fragmentColor = inputColor;
	
//	color = embos + ((inputColor - 0.1) * embos2);
//	color = embos + ((fragmentColor - 0.1) * embos2);
	
	float boarderRound = pow( UV.x, 2.0f );

	color = vec4(fragmentColor, boarderRound);

	color.a = 1.0;



//	color = texture( mySampler, UV ).rgb;
}
