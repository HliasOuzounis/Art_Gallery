#version 330 core
in vec4 FragPos;

struct Light{
    vec3 lightPos;
    float farPlane;
    mat4 shadowTransforms[6];
};
uniform Light light;

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - light.lightPos);
    
    // map to [0;1] range by dividing by farPlane
    lightDistance = lightDistance / light.farPlane;
    
    // write this as modified depth
    gl_FragDepth = lightDistance;
} 