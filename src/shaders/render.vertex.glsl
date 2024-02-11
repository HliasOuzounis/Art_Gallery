#version 330 core

// input vertex and UV coordinates, different for all executions of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;

// model view projection matrix 
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform vec3 viewPos;

out VS_OUT{   
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
    vec3 tangentFragPos;
    vec3 tangentViewPos;
} vs_out;

void main()
{
    gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0);

    vs_out.FragPos = vec3(M * vec4(vertexPosition_modelspace, 1.0));

    // vec3 normal1 = normalize(transpose(inverse(mat3(M))) * vertexNormal_modelspace);
    vs_out.Normal = normalize(vec3(M * vec4(vertexNormal_modelspace, 0.0)));

    vs_out.TexCoords = vertexUV;

    // calculate tangent/bitangent matrix
    vec3 T = normalize(mat3(M) * vec3(1.0, 0.0, 0.0));
    vec3 B = normalize(mat3(M) * vec3(0.0, 1.0, 0.0));
    vec3 N = normalize(mat3(M) * vec3(0.0, 0.0, 1.0));
    vs_out.TBN = mat3(T, B, N);

    mat3 TBN = vs_out.TBN;
    vs_out.tangentViewPos  = TBN * viewPos;
    vs_out.tangentFragPos  = TBN * vs_out.FragPos;
}
