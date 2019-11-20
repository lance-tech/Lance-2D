#version 330 core

in vec3 fragmentColor;
in vec2 UV;

out vec4 color;
uniform vec4 inputColor;
uniform float Time;

void main()
{
	// color = vec4(fragmentColor * inputColor.rgb, inputColor.a);
	color = vec4(0., 0., 1., 1.);
	// color = vec4(sin(Time), cos(Time), sin(Time), 1.0);
}
