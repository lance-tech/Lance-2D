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
// uniform sampler2D TextureSampler0;
// uniform vec3 LightPosition_worldspace;
// uniform vec3 ViewPos;
uniform float specularFactor;
uniform float Time;

void main()
{
	// vec3 textureColor = texture(TextureSampler0, UV).rgb;
	vec3 ambient = vec3(0.1, 0.1, 0.1) * inputColor.rgb;
    vec3 norm = normalize(Normal);
    vec3 lightPos = vec3(-1, 1, 1);
    vec3 lightDir = normalize(lightPos);  
    vec3 viewDir = normalize(eyeVector);
    vec3 reflectDir = -reflect(viewDir, norm);  
	float shininess = pow(35.0, specularFactor * 2);
    float spec = pow(max(dot(reflectDir, lightDir), 0.0), shininess);
    float lambert = max(dot(lightDir, norm), 0.0);
    vec3 fresnel = pow(1 - max(dot(viewDir, norm), 0.0), 4.0) * (1-lambert) * (inputColor.rgb * 0.5);
    vec3 result = (lambert * inputColor.rgb) + fresnel + (spec * specularFactor) + ambient;
    color = vec4(result, 1.0);
}