#version 330 core
out vec4 FragColor;

in vec3 color;

uniform sampler2D tex0;

void main()
{
	FragColor = vec4(0.5f, 0.6f, 0.5f, 0.5f);
}