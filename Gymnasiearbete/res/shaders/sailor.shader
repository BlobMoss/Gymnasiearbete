#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 normal;

out vec2 v_TexCoord;
out vec3 v_FragPos;
out vec4 v_Normal;

uniform mat4 u_MVP;
uniform mat4 u_ModelMatrix;
uniform mat4 u_NormalMatrix;

void main()
{
	gl_Position = u_MVP * position;

	v_TexCoord = texCoord;
	v_FragPos = vec3(u_ModelMatrix * position);
	v_Normal = u_NormalMatrix * normal;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 highlightColor;

in vec2 v_TexCoord;
in vec3 v_FragPos;
in vec4 v_Normal;

uniform sampler2D u_Texture;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

uniform vec4 u_HighlightColor;

uniform vec4 u_BeardColor;
uniform vec4 u_CoatColor;
uniform vec4 u_HatColor;

void main()
{
	// Color Attachment 0 (Texture Color)
	vec4 texColor = texture(u_Texture, v_TexCoord);

	if (texColor.b == 0.2) texColor = u_BeardColor;
	else if (texColor.b == 0.4) texColor = u_CoatColor;
	else if (texColor.b == 0.6) texColor = u_HatColor;

	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	// Ambient
	float ambientStrength = 0.3;

	vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	vec3 norm = normalize(v_Normal.xyz);
	vec3 lightDir = normalize(u_LightPos - v_FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular
	float specularStrength = 0.6;

	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8);
	vec3 specular = specularStrength * spec * lightColor;

	// Result
	vec3 result = (ambient + diffuse + specular) * texColor.xyz;
	color = vec4(result, 1.0);

	// Color Attachment 1 (Normal Color)
	normalColor = vec4(1.0);

	// Color Attachment 2 (Highlighted?)
	highlightColor = u_HighlightColor != vec4(0.0) ? u_HighlightColor : vec4(0.0f, 0.0f, 0.0f, 1.0f);
};