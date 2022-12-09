#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
//if we input vec2 this will implicit conversion to vec4

void main()
{
	gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;//"u_varient" mean this varient is a uniform varient

void main()
{
	color = u_Color;
};