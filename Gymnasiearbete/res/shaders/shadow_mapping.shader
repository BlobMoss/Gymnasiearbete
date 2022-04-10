#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 normal;

uniform mat4 u_LightSpaceMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = u_LightSpaceMatrix * u_ModelMatrix * position;
};

#shader fragment
#version 330 core

void main()
{
	
};