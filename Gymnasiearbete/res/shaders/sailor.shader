#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 normal;

out vec2 v_TexCoord;
out vec3 v_FragPos;
out vec4 v_Normal;
out vec4 v_FragPosLightSpace;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_LightSpaceMatrix;

void main()
{
	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * position;

	v_TexCoord = texCoord;
	v_FragPos = vec3(u_ModelMatrix * position);
	v_Normal = transpose(inverse(u_ModelMatrix)) * normal;
	v_FragPosLightSpace = u_LightSpaceMatrix * vec4(v_FragPos, 1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 highlightColor;

in vec2 v_TexCoord;
in vec3 v_FragPos;
in vec4 v_Normal;
in vec4 v_FragPosLightSpace;

uniform sampler2D u_Texture;
uniform sampler2D u_ShadowMap;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

uniform vec4 u_Color;

uniform vec3 u_LightColor;
uniform float u_AmbientStrength;
uniform float u_DiffuseStrength;
uniform float u_SpecularStrength;

uniform vec4 u_HighlightColor;

uniform vec4 u_BeardColor;
uniform vec4 u_CoatColor;
uniform vec4 u_HatColor;

float shadowCalc(float dotLightNormal)
{
	// transform from [-1, 1] range to [0, 1] range
	vec3 pos = v_FragPosLightSpace.xyz * 0.5 + 0.5;
	if (pos.z > 1.0) pos.z = 1.0;

	float bias = max(0.03 * (1.0 - dotLightNormal), 0.003);

	// PCF (percentage-closer filter)
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			float depth = texture(u_ShadowMap, pos.xy + vec2(x, y) * texelSize).r;
			shadow += (depth + bias) < pos.z ? 0.0 : 1.0;
		}
	}
	return shadow / 9.0;
}

void main()
{
	// Color Attachment 0 (Texture Color)
	vec4 texColor = texture(u_Texture, v_TexCoord);
	if (texColor.a == 0.0) discard;

	if (texColor.b == 0.2) texColor = u_BeardColor * u_Color;
	else if (texColor.b == 0.4) texColor = u_CoatColor * u_Color;
	else if (texColor.b == 0.6) texColor = u_HatColor * u_Color;
	else texColor *= u_Color;

	// Ambient
	vec3 ambient = u_AmbientStrength * u_LightColor;

	// Diffuse
	vec3 norm = normalize(v_Normal.xyz);
	vec3 lightDir = normalize(u_LightPos - v_FragPos);
	float dotLightNormal = dot(lightDir, norm);
	float diff = max(dotLightNormal, 0.0);
	vec3 diffuse = u_DiffuseStrength * diff * u_LightColor;

	// Specular
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
	vec3 specular = u_SpecularStrength * spec * u_LightColor;

	// Shadow
	float shadow = shadowCalc(dotLightNormal);

	// Color Attachment 0 (Texture Color)
	vec3 result = (shadow * (diffuse + specular) + ambient) * texColor.xyz;
	color = vec4(result, texColor.a);

	// Color Attachment 1 (Normal Color)
	normalColor = vec4(1.0);

	// Color Attachment 2 (Highlighted?)
	highlightColor = u_HighlightColor != vec4(0.0) ? u_HighlightColor : vec4(0.0f, 0.0f, 0.0f, 1.0f);
};