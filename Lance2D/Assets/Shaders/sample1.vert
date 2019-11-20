#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;

uniform vec2 Scale;
uniform mat4 MVP;
uniform float Time;

out vec3 fragmentColor;
out vec2 UV;

void main(){
	
	vec3 animModel = vec3(vertexPosition_modelspace.x + cos(Time * 1), 
							vertexPosition_modelspace.y + sin(Time * 1), 
							vertexPosition_modelspace.z);

	gl_Position =  MVP * vec4(animModel, 1);
	
	fragmentColor = vertexColor;
	UV = vertexUV;
}

