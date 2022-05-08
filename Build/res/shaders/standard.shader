#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 normal;

out vec2 v_TexCoord;
out vec4 v_Normal;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * position;

	v_TexCoord = texCoord;
	v_Normal = transpose(inverse(u_ModelMatrix)) * normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 highlightColor;

in vec2 v_TexCoord;
in vec4 v_Normal;

uniform sampler2D u_Texture;

uniform vec4 u_Color;

uniform vec4 u_HighlightColor;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord) * u_Color;
	if (texColor.a == 0.0) discard;
	color = texColor;

	vec4 nColor = vec4((normalize(v_Normal.xyz) * 0.5) + 0.5, 1.0);
	normalColor = nColor;

	// Color Attachment 2 (Highlighted?)
	highlightColor = u_HighlightColor != vec4(0.0) ? u_HighlightColor : vec4(0.0f, 0.0f, 0.0f, 1.0f);
};