#version 330 core

// Interpolated values from the vertex shaders
in vec3 FragPos;  
in vec3 Normal;  
in vec2 UV;
in vec3 eyeVector;
// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform vec4 inputColor;
uniform float specularFactor;
uniform float Time;

float lines(in vec2 pos, float b, float hardEdge){
    float scale = 10.0;
    pos *= scale;
	float amount = clamp(hardEdge, 0, 1);
    return smoothstep(amount * 0.5,
                    1 - (amount * 0.5),
                    abs((sin(pos.x*3.1415)+b*2.0))*.5);
}



void main()
{
	vec3 ambient = vec3(0.1, 0.1, 0.1) * inputColor.rgb;
    vec3 norm = normalize(Normal);
    vec3 lightPos = vec3(-50, 0, -50);
    vec3 lightDir = normalize(lightPos);  
    vec3 viewDir = normalize(eyeVector);
    vec3 reflectDir = -reflect(viewDir, norm);  
	float shininess = pow(35.0, specularFactor * 2);
    float spec = pow(max(dot(reflectDir, lightDir), 0.0), shininess);
    float lambert = max(dot(lightDir, norm), 0.0);
    vec3 fresnel = pow(1 - max(dot(viewDir, norm), 0.0), 4.0) * (1-lambert) * (inputColor.rgb * 0.5);
    vec3 result = (lambert * inputColor.rgb) + fresnel + (spec * specularFactor) + ambient;


    vec2 pos = vec2((UV.x * UV.y) + sin(Time * 0.1), UV.y) * vec2(3, 1.0);
    float pattern = lines(pos, 0.5, 0.9);

    // color = vec4(0.9922, 0.0157, 0.0157, 1.0);
    // color = vec4(pattern, pattern, pattern, 1.0);

    color = vec4(result * ((pattern + 0.25) * inputColor.rgb), 1.0);
}