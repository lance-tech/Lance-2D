#version 330 core

in vec3 fragmentColor;
in vec2 UV;

const int rowCount = 1;
const int columnCount = 4;
uniform vec4 inputColor;
uniform float Time;

out vec4 color;

vec4 AlphaBlend(vec4 src1, vec4 src2)
{
	vec4 s1 = vec4(src1.rgb * (1-src2.a), src1.a);
	return s1 + src2;
}

float circle(in vec2 _st, in float _radius, float count){
    
	vec2 dist = (_st - vec2(0.5, 1.0)) * vec2(1.0, count / 2.);
	return 1 - step(_radius, dot(dist, dist) * 4.0);
}

float shadowCircle(in vec2 _st, in float _radius, float count){
    
	vec2 dist = (_st - vec2(0.5, 1.0)) * vec2(1.0, count / 2.);
	return smoothstep(1., _radius + 0.25, dot(dist, dist) * 4.0);
}

vec4 RountTile(float x, float y, float c, vec3 color1, vec3 color2)
{
	vec2 st = vec2(fract((UV.x) * c)+x, fract(UV.y) + y);
	vec2 st2 = vec2(fract((UV.x) * c)-(1 - x), fract(UV.y) + y);
	
	float c1 = circle(vec2(st.x, st.y), 1.0, c);
	float c2 = circle(vec2(st2.x, st2.y), 1.0, c);

	float shadow1 = shadowCircle(vec2(st.x, st.y), 1.0, c);
	float shadow2 = shadowCircle(vec2(st2.x, st2.y), 1.0, c);

	float strip = step(fract((UV.x * (c / 2.0)) + (x - (x / 2)) ), 0.5);
	vec3 stripeColor = (color1 * strip) + (color2 * (1-strip));

	float shadow = shadow1*shadow2;
	float _circle = c1+c2;
	vec3 _circles = vec3(_circle * stripeColor);
	
	float _shadow2 = _circle + shadow;
	vec3 _shadows = vec3(_shadow2);

	return vec4(_circles * _shadows, 1-shadow);
}

vec4 Tiles(float rowCount, vec3 color1, vec3 color2)
{
	vec4 outColor;
	float yUnit = 1.0 / rowCount;
	for(int i = 0; i < rowCount; i++)
	{
		float x = i * 0.5;
		float moduleation = (x + Time) - (1.0 * floor((x + Time)/1.0));
		float y = i * yUnit;
		vec4 tileColor = RountTile(moduleation, y, rowCount, color1, color2);
		outColor = AlphaBlend(tileColor, outColor);
	}
	return outColor;
}

void main()
{
	color = Tiles(6.0, vec3(0.4549, 0.3882, 0.2902), vec3(0.2157, 0.2157, 0.2549));
}
