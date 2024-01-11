#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

struct Light{
    vec3 lightPos;
    float farPlane;
    mat4 shadowTransforms[6];
};
uniform Light light;

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle vertex
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = light.shadowTransforms[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}