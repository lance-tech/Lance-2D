#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;

uniform mat4 transform;

void main()
{
	vec3 outPos = position * vec3(1.0f, 1.0f, 1.0f);
    gl_Position = (transform * vec4(outPos, 1.0f)) * vec4(0.25f, 0.25f, 0.25f, 1.0f);

    ourColor = color;
}