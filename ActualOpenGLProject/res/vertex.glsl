#version 460 core // version 4.60 

layout(location = 0) in vec4 position;

void main() 
{
	gl_Position = position;
}
