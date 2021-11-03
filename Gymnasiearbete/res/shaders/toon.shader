#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_ColorTexture;
uniform sampler2D u_NormalTexture;
uniform sampler2D u_DepthTexture;
uniform float u_Near;
uniform float u_Far;

float LinearizeDepth(float z)
{
    float n = u_Near;
    float f = u_Far;
    return (2.0 * n) / (f + n - z * (f - n));
}

mat3 sobel_y = mat3(
    1.0, 0.0, -1.0,
    2.0, 0.0, -2.0,
    1.0, 0.0, -1.0
);

mat3 sobel_x = mat3(
    1.0, 2.0, 1.0,
    0.0, 0.0, 0.0,
    -1.0, -2.0, -1.0
);


float SobelFilterNormals()
{
    mat3 I;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vec3 sample = texelFetch(u_NormalTexture, ivec2(gl_FragCoord / 2) + ivec2(i - 1, j - 1), 0).rgb;
            I[i][j] = length(sample);
        }
    }

    float gx = dot(sobel_x[0], I[0]) + dot(sobel_x[1], I[1]) + dot(sobel_x[2], I[2]);
    float gy = dot(sobel_y[0], I[0]) + dot(sobel_y[1], I[1]) + dot(sobel_y[2], I[2]);

    float g = sqrt(pow(gx, 2.0) + pow(gy, 2.0));

    return g;
}

float SobelFilterDepth()
{
    mat3 I;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vec3 sample = texelFetch(u_DepthTexture, ivec2(gl_FragCoord / 2) + ivec2(i - 1, j - 1), 0).rgb;
            I[i][j] = LinearizeDepth(length(sample));
        }
    }

    float gx = dot(sobel_x[0], I[0]) + dot(sobel_x[1], I[1]) + dot(sobel_x[2], I[2]);
    float gy = dot(sobel_y[0], I[0]) + dot(sobel_y[1], I[1]) + dot(sobel_y[2], I[2]);
     
    float g = sqrt(pow(gx, 2.0) + pow(gy, 2.0));

    return g;
}

void main()
{
    vec4 texColor = texture(u_ColorTexture, v_TexCoord);

    float sobelNormals = SobelFilterNormals();
    float sobelDepth = SobelFilterDepth();

    if (sobelNormals < 0.7) sobelNormals = 0.0; else sobelNormals = 1.0;
    if (sobelDepth < 0.05) sobelDepth = 0.0; else sobelDepth = 1.0;

    float result = 1.0 - clamp(sobelNormals + sobelDepth, 0, 1);

    color = vec4(texColor.xyz * result, 1.0);

    //color = vec4(vec3(result), 1.0);
    //color = texture(u_ColorTexture, v_TexCoord);
    //color = texture(u_NormalTexture, v_TexCoord);
    //color = vec4(vec3(LinearizeDepth(texture(u_DepthTexture, v_TexCoord).r)), 1.0f);
};