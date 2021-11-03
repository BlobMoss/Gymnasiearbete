#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 normal;

out vec2 v_TexCoord;
out vec4 v_Normal;

uniform mat4 u_MVP;
uniform mat4 u_NormalMatrix;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_Normal = u_NormalMatrix * normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normalColor;

in vec2 v_TexCoord;
in vec4 v_Normal;

uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor;

	vec4 nColor = vec4((normalize(v_Normal.xyz) * 0.5) + 0.5, 1.0);
	normalColor = nColor;
};