#version 330 core

in vec4 vertex_position_cameraspace;
in vec4 vertex_normal_cameraspace;
in vec4 light_position_cameraspace;
// in vec2 vertex_UV;
// in vec4 vertex_position_lightspace;

//lighting
struct Light {
    vec3 lightPosition;
    vec4 La;
    vec4 Ld;
    vec4 Ls;
    // mat4 lightVP;
    // float lightIntensity;
};
uniform Light light;

uniform vec3 color;

// output data
out vec4 fragmentColor;

vec4 phong(float visibility);


void main()
{
    fragmentColor = phong(1.0);
}

vec4 phong(float visibility){
    vec4 Ia = vec4(color, 1) * light.La;

    vec4 N = normalize(vertex_normal_cameraspace);
    vec4 L = normalize(light_position_cameraspace - vertex_position_cameraspace);
    float cosTheta = clamp(dot(N, L), 0, 1);
    vec4 Id = light.Ld * vec4(color, 1) * cosTheta;

    vec4 R = reflect(-L, N);
    vec4 V = normalize(-vertex_position_cameraspace);
    float cosAlpha = clamp(dot(R, V), 0, 1);
    vec4 Is = light.Ls * vec4(color, 1) * pow(cosAlpha, 1);

    return visibility * (Ia + Id + Is);
}

