#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelsapce;

// Output data ; will be interpolated for each fragment
out vec3 FragPos;
out vec3 Normal;
out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 LightPosition_worldspace;

uniform float Time;


void main()
{
	vec3 vp = vertexPosition_modelspace;

	float mask = 1-vertexUV.y;

	float wave0 = sin((Time * 10) + (vertexUV.y * 20)) * 0.05;
	float wave1 = sin((Time * 2) + (vertexUV.y * 5)) * 0.15;
	float wave12 = sin((Time) + (vertexUV.y * 2)) * 0.55;

	float wave2 = sin((Time * 2) + (vertexUV.y * 10)) * 0.55;

	float waveX = sin((Time * 2) + (vertexUV.x * 5)) * 0.1;

	float xWave = (mask * wave0) + (mask * wave1) + (mask * wave12);

	float yWave = mask * waveX;
	
	float zWave = mask * wave2;


	FragPos = vec3(M * vec4(vp.x + xWave, vp.y + yWave, vp.z + zWave, 1.0));
	Normal = mat3(transpose(inverse(M))) * vertexNormal_modelsapce;  
	UV = vertexUV;

	gl_Position = P * V * vec4(FragPos, 1);
}
