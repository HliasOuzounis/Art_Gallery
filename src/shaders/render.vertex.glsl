#version 330 core

// input vertex and UV coordinates, different for all executions of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexTangent_modelspace;
layout(location = 4) in vec3 vertexBitangent_modelspace;

// model view projection matrix 
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

struct Light {
    vec3 lightPos;
    vec4 La;
    vec4 Ld;
    vec4 Ls;
    float farPlane;
    float lightIntensity;
};
uniform Light light;

uniform vec3 viewPos;

// Output data will be interpolated for each fragment.
out VS_OUT{   
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
    vec3 tangentNormal;
    vec3 tangentFragPos;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
} vs_out;

void main()
{
    gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0);

    vs_out.FragPos = vec3(M * vec4(vertexPosition_modelspace, 1.0));
    vs_out.Normal = normalize(vec3(M * vec4(vertexNormal_modelspace, 0.0)));


    vs_out.TexCoords = vertexUV;

    // calculate tangent/bitangent matrix

    mat3 normalMatrix = transpose(inverse(mat3(M)));
    vec3 T = normalize(normalMatrix * vertexTangent_modelspace);
    vec3 B = normalize(normalMatrix * vertexBitangent_modelspace);
    vec3 N = normalize(normalMatrix * vertexNormal_modelspace);
    
    vs_out.TBN = transpose(mat3(T, B, N));
    vs_out.tangentNormal = vs_out.TBN * vs_out.Normal;
    vs_out.tangentFragPos = vs_out.TBN * vs_out.FragPos;
    vs_out.tangentLightPos = vs_out.TBN * light.lightPos;
    vs_out.tangentViewPos = vs_out.TBN * viewPos;
}
