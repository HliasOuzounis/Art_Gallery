#version 330 core

in vec4 vertex_position_cameraspace;
in vec4 vertex_normal_cameraspace;
in vec4 light_position_cameraspace;
in vec2 vertex_UV;
in vec4 vertex_position_lightspace;

//lighting
struct Light {
    vec3 lightPosition;
    vec4 La;
    vec4 Ld;
    vec4 Ls;
    mat4 lightVP;
    // float lightIntensity;
};
uniform Light light;

struct Material {
    vec4 Ka;
    vec4 Kd;
    vec4 Ks;
    float Ns;
};
uniform Material material;

uniform int useTexture = 0;
uniform sampler2D diffuseColorSampler;
uniform sampler2D specularColorSampler;

uniform sampler2D shadowMapSampler;

// output data
out vec4 fragmentColor;

vec4 phong(float visibility);
float shadowCalculation(vec4 fragPositionLightspace);

vec4 Kd, Ks, Ka;
float Ns;

void main()
{   
    float visibility = 1 - shadowCalculation(vertex_position_lightspace);
    fragmentColor = phong(visibility);
}

float shadowCalculation(vec4 fragPositionLightspace){
    vec3 projCoords = fragPositionLightspace.xyz / fragPositionLightspace.w;

    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMapSampler, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadowFactor = 0.0;
    float bias = 0.0005;
    vec2 depthMap_dimensions = textureSize(shadowMapSampler, 0);
    vec2 texelSize = 1.0 / depthMap_dimensions;
    for(int x = -1; x <= 1; x++ ){
        for(int y = -1; y <= 1; y++ ){
            float pcfDepth = texture(shadowMapSampler, projCoords.xy + vec2(x, y) * texelSize).r;
            shadowFactor += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadowFactor /= 9.0;

    if (projCoords.z > 1.0)
        shadowFactor = 0.0;

    return shadowFactor;
}

vec4 phong(float visibility){
    if (useTexture == 1){
        Ka = vec4(0.05 * Kd.rgb, 1.0);
        Kd = texture(diffuseColorSampler, vertex_UV);
        Ks = texture(specularColorSampler, vertex_UV);
        Ns = 50;
    } else {
        Ks = material.Ks;
        Kd = material.Kd;
        Ka = material.Ka;
        Ns = material.Ns;
    }
    vec4 Ia = Ka * light.La;

    vec4 N = normalize(vertex_normal_cameraspace);
    vec4 L = normalize(light_position_cameraspace - vertex_position_cameraspace);
    float cosTheta = clamp(dot(N, L), 0, 1);
    vec4 Id = light.Ld * Kd * cosTheta;

    vec4 R = reflect(-L, N);
    vec4 V = normalize(-vertex_position_cameraspace);
    float cosAlpha = clamp(dot(R, V), 0, 1);
    vec4 Is = light.Ls * Ks * pow(cosAlpha, Ns);

    vec4 finalColor = (Ia + Id + Is) * visibility;
    finalColor.a = 1;
    return finalColor;
}

