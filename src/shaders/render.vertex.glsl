#version 330 core

// input vertex and UV coordinates, different for all executions of this shader
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in vec3 vertexTangent;

// model view projection matrix 
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

// Output data will be interpolated for each fragment.
out VS_OUT{   
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main()
{
    gl_Position = P * V * M * vec4(vertexPosition_modelspace, 1.0);

    vs_out.FragPos = vec3(M * vec4(vertexPosition_modelspace, 1.0));

    // vec3 normal1 = normalize(transpose(inverse(mat3(M))) * vertexNormal_modelspace);
    vs_out.Normal = normalize(vec3(M * vec4(vertexNormal_modelspace, 0.0)));

    vs_out.TexCoords = vertexUV;
}
