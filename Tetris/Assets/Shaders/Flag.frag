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


float circle(in vec2 _st, in float _radius){
    vec2 dist = _st-vec2(0.5);
	return 1.-step(_radius+(_radius * 10), dot(dist,dist)*4.0);
}

void main()
{
    

	vec3 textureColor = texture(TextureSampler0, UV).rgb;

	vec3 ambient = vec3(0.1, 0.1, 0.1) * inputColor.rgb;
	// diffuse 
    vec3 norm = normalize(Normal);

	vec3 lightDir = normalize(LightPosition_worldspace - FragPos);
    //vec3 lightDir = normalize(-LightPosition_worldspace);  
    float diff = max(dot(norm, lightDir), 0.0);
	vec3 lightDiffuse = vec3(1.0, 1.0, 1.0);
    vec3 diffuse = lightDiffuse * diff * textureColor;  
    
    // specular
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
	
	float shininess = 32.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	vec3 lightSpecular = vec3(1.0, 1.0, 1.0);
	int materialSpecular = 1;
    //vec3 specular = lightSpecular * spec * texture(materialSpecular, UV).rgb;  
	vec3 specular = lightSpecular * spec;
    vec3 result = ambient + diffuse;// + specular;
    // color = vec4(result, 1.0);
    
    vec2 iResolution = vec2(1.0, 1.0);
	vec2 uv = UV.xy / iResolution.xy;

    float maskY = pow(UV.y * 4, 5);
    
    float noise2 = pow(textureColor.x, 6) * 5;
    vec3 noiseColor = vec3(noise2);

    vec3 col = texture(TextureSampler0, vec2(UV.x * 2, UV.y * 0.75)).rgb * 0.95;
	float bright = 0.33333 * (col.r + col.g + col.b);
	float b = mix(0.0, 1.0, step(0.5, bright));

    vec3 col2 = texture(TextureSampler0, vec2(UV.x * 0.25, UV.y * 0.5)).rgb * 0.9;
	float bright2 = 0.33333 * (col2.r + col2.g + col2.b);
	float b2 = mix(0.0, 1.0, step(0.5, bright2));

    float amount = 2;
    float softness = 2;
    float maskL = pow(UV.x * amount, softness);
    float maskR = pow((1-UV.x) * amount, softness);
    
    float MaskLR = maskL * maskR;
    float InvMaskLR = 1-(maskL * maskR);

    float finalMask = InvMaskLR + MaskLR;

    float stepL = step(0.05, UV.x);
    float stepR = step(0.05, 1-UV.x);
    float m1 = (b2 * pow(MaskLR * 15, 2)) * step(0.5, InvMaskLR) + step(0.5, MaskLR);
    float m2 = (b * stepL * stepR) * step(0.5, InvMaskLR) + step(0.5, MaskLR);

    float c1 = circle(uv, 0.09) + step(0.5, UV.y);
    float c2 = circle(uv, 0.07) + step(0.5, UV.y);

    float alpha = (m1 + m2) * c1 * (b + c2);
    // vec3 finalColor = vec3(1.0, 1.0, 1.0) * c1 * (b + c2);
    vec3 finalColor = vec3(1.0, 1.0, 1.0);

    color = vec4(result, alpha);
    // color = vec4(MaskLR, MaskLR, MaskLR, 1.0);


    
    // color = vec4(vec3(b), 1.0);
}