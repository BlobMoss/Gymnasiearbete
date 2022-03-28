#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec4 normal;

out vec2 v_TexCoord;
out vec3 v_FragPos;
out vec4 v_Normal;
out vec4 v_FlatNormal;

uniform mat4 u_MVP;
uniform mat4 u_ModelMatrix;
uniform mat4 u_NormalMatrix;

uniform float u_Time;
uniform vec4 u_Offset;

const float pi = 3.14159;
const float waterHeight = -0.67;
const float amplitude[4] = float[4](0.26, 0.21, 0.15, 0.12);
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
	gl_Position = u_MVP * pos;

	v_TexCoord = texCoord;
	v_FragPos = vec3(u_ModelMatrix * pos);

	vec4 nor = vec4(waveNormal(pos.x + u_Offset.x, pos.z + u_Offset.z), 0.0);
	v_Normal = u_NormalMatrix * normalize(nor);
	v_FlatNormal = normalize(v_Normal + normal);
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

uniform sampler2D u_Texture;

uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

void main()
{
	// Color Attachment 0 (Texture Color)
	vec4 texColor = texture(u_Texture, v_TexCoord);

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
	color = vec4(result, 0.5);

	// Color Attachment 1 (Normal Color)
	vec4 nColor = vec4((normalize(v_FlatNormal.xyz) * 0.5) + 0.5, 1.0);
	normalColor = nColor;

	// Color Attachment 2 (Highlighted?)
	highlightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
};