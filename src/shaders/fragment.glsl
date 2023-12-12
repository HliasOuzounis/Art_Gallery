#version 330 core

in vec4 vertex_position_cameraspace;
in vec4 vertex_normal_cameraspace;
in vec4 light_position_cameraspace;
// in vec2 vertex_UV;
in vec4 vertex_position_lightspace;

//lighting
uniform vec4 lightColor;
uniform float lightIntensity;

uniform vec3 color;

// output data
out vec4 fragmentColor;

vec4 phong(float visibility);


void main()
{
    fragmentColor = phong(1.0);
}

vec4 phong(float visibility){
    vec4 Ia = lightColor * vec4(color, 1);

    vec4 N = normalize(vertex_normal_cameraspace);
    vec4 L = normalize(light_position_cameraspace - vertex_position_cameraspace);
    float cosTheta = clamp(dot(N, L), 0, 1);
    vec4 Id = lightColor * vec4(color, 1) * cosTheta;

    vec4 R = reflect(-L, N);
    vec4 V = normalize(-vertex_position_cameraspace);
    float cosAlpha = clamp(dot(R, V), 0, 1);
    vec4 Is = lightColor * vec4(color, 1) * pow(cosAlpha, 5);

    return visibility * (Ia + Id + Is);
}