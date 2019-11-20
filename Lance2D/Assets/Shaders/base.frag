#version 330 core
in vec3 ourColor;

out vec4 color;
uniform vec3 inColor;


void main()
{
	color = vec4(ourColor * inColor, 1.0f);
}