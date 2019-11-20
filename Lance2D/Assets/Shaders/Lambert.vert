#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_modelsapce;

// Output data ; will be interpolated for each fragment
out vec2 UV;
out vec3 Position_worldspace;
out float mDiffuse;
out vec3 mReflection;
out vec3 mViewDir;


// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 LightPosition_worldspace;

void main(){
	gl_Position = MVP * vec4(vertexPosition_modelspace * 3, 1);

	Position_worldspace = (M * vec4(vertexPosition_modelspace, 1)).xyz;

	vec3 lightDir = normalize(Position_worldspace - LightPosition_worldspace);

	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace, 1)).xyz;
	mViewDir = vec3(0, 0, 0) - vertexPosition_cameraspace;

	// Position_worldspace = (V * vec4(Position_worldspace, 1)).xyz;
	// Position_worldspace = (P * vec4(Position_worldspace, 1)).xyz;

	vec3 worldNormal = (M * vec4(vertexNormal_modelsapce, 1)).xyz;
	worldNormal = normalize(worldNormal);

	mDiffuse = dot(-lightDir, worldNormal);
	mReflection = reflect(lightDir, worldNormal);

	UV = vertexUV;
}

