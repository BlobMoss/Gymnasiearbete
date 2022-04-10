#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 normal;

out vec2 v_TexCoord;
out vec3 v_FragPos;
out vec4 v_Normal;
out vec4 v_FlatNormal;
out vec4 v_FragPosLightSpace;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;
uniform mat4 u_LightSpaceMatrix;

uniform float u_Time;
uniform vec4 u_Offset;

const float pi = 3.14159;
const float waterHeight = -0.67;
const float amplitude[4] = float[4](0.23, 0.20, 0.15, 0.12);
const float wavelength[4] = float[4](20.1, 13.5, 7.3, 5.4);
const float speed[4] = float[4](1.0, 2.0, 3.0, 4.0);
const vec2 direction[4] = vec2[4](vec2(0.97, 0.22), vec2(0.95, -0.31), vec2(0.92, 0.38), vec2(0.56, 0.83));

float wave(int i, float x, float y) {
	float frequency = 2.0 * pi / wavelength[i];
	float phase = speed[i] * frequency;
	float theta = dot(direction[i], vec2(x, y));
	return amplitude[i] * sin(theta * frequency + u_Time * phase);
}

float waveHeight(float x, float y) {
	float height = 0.0;
	for (int i = 0; i < 4; ++i)
		height += wave(i, x, y);
	return height;
}

float dWavedx(int i, float x, float y) {
	float frequency = 2 * pi / wavelength[i];
	float phase = speed[i] * frequency;
	float theta = dot(direction[i], vec2(x, y));
	float A = amplitude[i] * direction[i].x * frequency;
	return A * cos(theta * frequency + u_Time * phase);
}

float dWavedy(int i, float x, float y) {
	float frequency = 2 * pi / wavelength[i];
	float phase = speed[i] * frequency;
	float theta = dot(direction[i], vec2(x, y));
	float A = amplitude[i] * direction[i].y * frequency;
	return A * cos(theta * frequency + u_Time * phase);
}

vec3 waveNormal(float x, float y) {
	float dx = 0.0;
	float dy = 0.0;
	for (int i = 0; i < 4; ++i) {
		dx += dWavedx(i, x, y);
		dy += dWavedy(i, x, y);
	}
	vec3 n = vec3(-dx, -dy, 1.0);
	return normalize(n);
}

void main()
{
	vec4 pos = position;
	pos.y = waveHeight(pos.x + u_Offset.x, pos.z + u_Offset.z) + waterHeight;

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * pos;

	v_TexCoord = texCoord;
	v_FragPos = vec3(u_ModelMatrix * pos);

	vec4 nor = vec4(waveNormal(pos.x + u_Offset.x, pos.z + u_Offset.z), 0.0);
	v_Normal = transpose(inverse(u_ModelMatrix)) * normalize(nor);
	v_FlatNormal = normalize(v_Normal + normal);

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
in vec4 v_FlatNormal;
in vec4 v_FragPosLightSpace;

uniform sampler2D u_Texture;
uniform sampler2D u_ShadowMap;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

uniform vec3 u_LightColor;
uniform float u_AmbientStrength;
uniform float u_SpecularStrength;

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

	// Ambient
	vec3 ambient = u_AmbientStrength * u_LightColor;

	// Diffuse
	vec3 norm = normalize(v_Normal.xyz);
	vec3 lightDir = normalize(u_LightPos - v_FragPos);
	float dotLightNormal = dot(lightDir, norm);
	float diff = max(dotLightNormal, 0.0);
	vec3 diffuse = diff * u_LightColor;

	// Specular
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
	vec3 specular = u_SpecularStrength * spec * u_LightColor;

	// Shadow
	float shadow = shadowCalc(dotLightNormal);

	// Color Attachment 0 (Texture Color)
	vec3 result = (shadow * (diffuse + specular) + ambient) * texColor.xyz;
	color = vec4(result, 0.5);

	// Color Attachment 1 (Normal Color)
	normalColor = vec4(1.0);

	// Color Attachment 2 (Highlighted?)
	highlightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
};