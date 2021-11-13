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
uniform int u_TexWidth;
uniform int u_TexHeight;
uniform float u_Near;
uniform float u_Far;

float LinearizeDepth(float z)
{
    float n = u_Near;
    float f = u_Far;
    return (2.0 * n) / (f + n - z * (f - n));
}

void MakeKernel(inout vec4 n[9], sampler2D tex, vec2 coord)
{
    float w = 1.0 / u_TexWidth;
    float h = 1.0 / u_TexHeight;

    n[0] = texture2D(tex, coord + vec2(-w, -h));
    n[1] = texture2D(tex, coord + vec2(0.0, -h));
    n[2] = texture2D(tex, coord + vec2(w, -h));
    n[3] = texture2D(tex, coord + vec2(-w, 0.0));
    n[4] = texture2D(tex, coord);
    n[5] = texture2D(tex, coord + vec2(w, 0.0));
    n[6] = texture2D(tex, coord + vec2(-w, h));
    n[7] = texture2D(tex, coord + vec2(0.0, h));
    n[8] = texture2D(tex, coord + vec2(w, h));
}

float SobelFilter(bool depth)
{
    vec4 n[9];
    if (depth) MakeKernel(n, u_DepthTexture, v_TexCoord);
    else MakeKernel(n, u_NormalTexture, v_TexCoord);

    vec4 sobelEdgeH = n[2] + (2.0 * n[5]) + n[8] - (n[0] + (2.0 * n[3]) + n[6]);
    vec4 sobelEdgeV = n[0] + (2.0 * n[1]) + n[2] - (n[6] + (2.0 * n[7]) + n[8]);
    vec4 sobel = sqrt((sobelEdgeH * sobelEdgeH) + (sobelEdgeV * sobelEdgeV));

    //return length(sobel);
    return depth ? LinearizeDepth(length(sobel)) : length(sobel);
}

void main()
{
    vec4 texColor = texture(u_ColorTexture, v_TexCoord);

    float sobelNormals = SobelFilter(false) < 0.97 ? 1.0 : 0.0;
    float sobelDepth = SobelFilter(true) < 0.02 ? 1.0 : 0.0;

    color = vec4(texColor.rgb * sobelNormals * sobelDepth, 1.0);

    //color = texture(u_ColorTexture, v_TexCoord);
    //color = texture(u_NormalTexture, v_TexCoord);
    //color = vec4(vec3(texture(u_DepthTexture, v_TexCoord).r), 1.0f);
    //color = vec4(vec3(LinearizeDepth(texture(u_DepthTexture, v_TexCoord).r)), 1.0f);
};