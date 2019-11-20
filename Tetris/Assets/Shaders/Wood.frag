#version 330 core

// Interpolated values from the vertex shaders
in vec3 FragPos;  
in vec3 Normal;  
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform vec4 inputColor;
uniform sampler2D TextureSampler0;
uniform vec3 LightPosition_worldspace;
uniform vec3 ViewPos;
uniform float Time;


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
		float moduleation = (x + 0) - (1.0 * floor((x + 0)/1.0));
		float y = i * yUnit;
		vec4 tileColor = RountTile(moduleation, y, rowCount, color1, color2);
		outColor = AlphaBlend(tileColor, outColor);
	}
	return outColor;
}

float diamond(vec2 uv, int row, int column)
{
	float UVY = fract(uv.y * row);
	float lineX = abs((fract(uv.x * column) * 2) - 1) * 0.5;
	float dia1 = UVY + lineX;
	float hDia1 = smoothstep(dia1, dia1 + 0.1, 0.5);

	float dia2 = (1-UVY) + lineX;
	float hDia2 = smoothstep(dia2, dia2 + 0.1, 0.5);

	float dia = hDia1 + hDia2;

	return smoothstep(dia1, dia1 + 0.8, 0.5) + smoothstep(dia2, dia2 + 0.8, 0.5);
}

void main()
{
	vec3 ambient = vec3(0.1, 0.1, 0.1) * inputColor.rgb;
	// diffuse 
    vec3 norm = normalize(Normal);

	vec3 lightDir = normalize(LightPosition_worldspace - FragPos);
    //vec3 lightDir = normalize(-LightPosition_worldspace);  
    float diff = max(dot(norm, lightDir), 0.0);
	vec3 lightDiffuse = vec3(1.0, 1.0, 1.0);
    // vec3 diffuse = lightDiffuse * diff * textureColor;  
    vec3 diffuse = lightDiffuse * diff * inputColor.rgb;  
    
    // specular
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
	
	float shininess = 32.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 lightSpecular = vec3(1.0, 1.0, 1.0);
	int materialSpecular = 1;
    //vec3 specular = lightSpecular * spec * texture(materialSpecular, UV).rgb;  
	vec3 specular = lightSpecular * spec;
    vec3 result = ambient + diffuse + specular;
    
	int row = 2;
	int col = 7;
	float dia1 = diamond(UV, row, col);
	float dia2 = diamond(vec2(UV.x + (1.0/(col * 2.0)), UV.y + (1.0/(row * 2.0))), row, col);
	float diamond1 = dia1 + dia2;

	// float diaLine = clamp(1-(pow(diamond1, 0.5) * 5), 0, 1);
	float diaLine = 1-(pow(diamond1, 0.5) * 5);
	vec3 DiaTile = vec3(abs(diaLine));

	color = vec4(result * inputColor.rgb * DiaTile, 1.0);
}