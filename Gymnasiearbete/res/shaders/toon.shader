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
uniform sampler2D u_HighlightTexture;
uniform sampler2D u_DepthTexture;
uniform int u_TexWidth;
uniform int u_TexHeight;
uniform float u_Near;
uniform float u_Far;

float LinearizeDepth(float z)
{
    return (z - u_Near) / (u_Far - u_Near);
}

void MakeOutlineKernel(inout vec4 n[8], sampler2D tex, vec2 coord)
{
    float w = 1.0 / u_TexWidth;
    float h = 1.0 / u_TexHeight;

    n[0] = texture2D(tex, coord + vec2(-w, -h));
    n[1] = texture2D(tex, coord + vec2(0.0, -h));
    n[2] = texture2D(tex, coord + vec2(w, -h));
    n[3] = texture2D(tex, coord + vec2(-w, 0.0));
    n[4] = texture2D(tex, coord + vec2(w, 0.0));
    n[5] = texture2D(tex, coord + vec2(-w, h));
    n[6] = texture2D(tex, coord + vec2(0.0, h));
    n[7] = texture2D(tex, coord + vec2(w, h));
}

bool OutlineFilter()
{
    vec4 n[8];
    MakeOutlineKernel(n, u_HighlightTexture, v_TexCoord);

    int neighbors = 0;
    for (int i = 0; i < 8; i++)
    {
        if (n[i] == vec4(1.0f)) neighbors++;
    }
    return neighbors != 0 && neighbors != 8;
}

void MakeSobelKernel(inout vec4 n[9], sampler2D tex, vec2 coord)
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

float SobelFilter(int type)
{
    vec4 n[9];
    if (type == 0) MakeSobelKernel(n, u_NormalTexture, v_TexCoord);
    if (type == 1) MakeSobelKernel(n, u_DepthTexture, v_TexCoord);

    if (n[4] == vec4(1.0) && type != 0) return 0.0f;

    vec4 sobelEdgeH = n[2] + (2.0 * n[5]) + n[8] - (n[0] + (2.0 * n[3]) + n[6]);
    vec4 sobelEdgeV = n[0] + (2.0 * n[1]) + n[2] - (n[6] + (2.0 * n[7]) + n[8]);
    vec4 sobel = sqrt((sobelEdgeH * sobelEdgeH) + (sobelEdgeV * sobelEdgeV));

    return length(sobel);
    //return depth ? LinearizeDepth(length(sobel)) : length(sobel);
}

void main()
{
    vec4 texColor = texture(u_ColorTexture, v_TexCoord);

    bool outline = OutlineFilter();
    float sobelNormals = SobelFilter(0) < 0.97 ? 1.0 : 0.4;
    float sobelDepth = SobelFilter(1) < 0.04 ? 1.0 : 0.2;

    if (outline)
    {
        color = vec4(0.988, 0.462, 0.031, 1.0f);
    }
    else
    {
        color = vec4(texColor.rgb * sobelNormals * sobelDepth, 1.0);
    }

    //color = texture(u_ColorTexture, v_TexCoord);
    //color = texture(u_NormalTexture, v_TexCoord);
    //color = texture(u_HighlightTexture, v_TexCoord);
    //color = vec4(vec3(texture(u_DepthTexture, v_TexCoord).r), 1.0f);
    //color = vec4(vec3(LinearizeDepth(texture(u_DepthTexture, v_TexCoord).r)), 1.0f);
};