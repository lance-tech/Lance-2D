#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in float mDiffuse;
in vec3 mReflection;
in vec3 mViewDir;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform vec4 inputColor;
uniform sampler2D TextureSampler0;
uniform vec3 LightPosition_worldspace;

void main()
{
	vec3 textureColor = texture(TextureSampler0, UV).rgb;

	float diffuse = clamp(mDiffuse, 0, 1);
	vec3 reflection = normalize(mReflection);
	vec3 viewDir = normalize(mViewDir); 
	float specular = 0;
	
	if (diffuse > 0) {
		specular = clamp( dot(reflection, viewDir), 0, 1);
		specular = pow(specular, 50.0f) * 1.0;
	}

	
	vec3 ambient = vec3(0.1, 0.1, 0.1) * textureColor;
	color = ambient + (textureColor * diffuse * inputColor.rgb) + specular;

//	vec3 ambient = vec3(0.15, 0.15, 0.15);
//	color = ambient + vec3(1.0, 1.0, 1.0) * diffuse + specular;
}