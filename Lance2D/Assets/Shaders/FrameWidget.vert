#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform vec3 inputPosition;
uniform vec2 inputSize;

out vec3 fragmentColor;
out vec2 UV;

void main(){

	// Output position of the vertex, in clip space : MVP * position
//vec3 scaleVertex = vec3(vertexPosition_modelspace.x * inputSize.x, 
//							vertexPosition_modelspace.y * inputSize.y, 
//							vertexPosition_modelspace.z);
//	vec3 outVertex = scaleVertex + inputPosition;
//	gl_Position =  MVP * vec4(outVertex, 1);

	gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);

	fragmentColor = vertexColor;
	UV = vertexUV;
}

