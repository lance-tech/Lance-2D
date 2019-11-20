#version 330 core

in vec3 fragmentColor;
in vec2 UV;

uniform int rowCount;
uniform int columnCount;
uniform vec4 inputColor;
uniform float Time;

out vec4 color;


float circle(in vec2 _st, in float _radius, float _animX, float _animY){
    vec2 dist = _st-vec2(0.5);
	
	float radX = _radius * (_animX * 10);
	float radY = _radius * (_animY * 10);

	return 1.-smoothstep(_radius-(radX),
                         _radius+(radY),
                         dot(dist,dist)*4.0);
}

void main()
{
	float lineWidth = 0.9;
	float lineOffset = (1-lineWidth) * 0.5;
	float stripeX = floor(fract((UV.x * columnCount) + lineOffset) + lineWidth);
	float stripeY = floor(fract((UV.y * rowCount) + lineOffset) + lineWidth);
	
	float gridBoard = stripeX * stripeY;

//	float stripeY = floor(fract(UV.y * stepCountRow) + 0.5);

	float inv_stripeX = 1 - stripeX;
	float inv_stripeY = 1 - stripeY;

	float check1 = stripeX + stripeY;
	float check2 = inv_stripeX + inv_stripeY;

	// float checkBoard = check1 * check2;
	float checkBoard = inv_stripeX * inv_stripeY;
	

//	float gradientColor = pow( UV.x, 2.0f );
//	color = gradientColor * vec3(1, 1, 1);
//	color.rgb = inputColor.rgb * ((gridBoard) * fragmentColor);
	// color.rgb = inputColor.rgb * ((gridBoard + 0.9) * fragmentColor)*0.2;

	float animX = (sin(Time + (UV.y * 5)) + 1) * 0.5;
	float animY = (sin(Time + (UV.x * 2)) + 1) * 0.5;

	// float animX = sin(Time + (UV.y * 20)) * 0.1;
	// float animY = sin(Time + (UV.x * 20)) * 0.1;
	// vec2 st = vec2(fract(UV.x * columnCount) + animX, fract(UV.y * rowCount) + animY) / iResolution.xy;

	vec2 iResolution = vec2(1.0, 1.0);
	// vec2 st = vec2(UV.x * 1, UV.y * 1) / iResolution.xy;

	vec2 st = vec2(fract(UV.x * columnCount), fract(UV.y * rowCount)) / iResolution.xy;

	vec3 circle1 = vec3(circle(st, 0.01 , animX, animY));
	vec3 bg = vec3(1-animY, 1-animX, animY) * 0.05;

	// vec3 alpha = circle1.xyz;
	// color.rgb = alpha.ggg;
	color.rgb = vec3(circle1.x * animX, circle1.y * animY, circle1.z * animY * animX) + bg;
	color.a = 1.0;

//	color = (1-pow(1-UV.x, 20)) * (1 - pow(UV.x, 20)) *  fragmentColor;
//	color = texture( mySampler, UV ).rgb;
}
