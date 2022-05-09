// vertexShaderSource 
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 Color;
uniform mat4 camMatrix;
uniform mat4 model;
void main()
{
	Color = color;
	gl_Position = camMatrix * model * vec4(position, 1.0);
}