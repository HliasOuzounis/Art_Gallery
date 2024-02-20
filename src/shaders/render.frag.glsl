#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
} fs_in;

//lighting
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

uniform mat4 M;

struct Material {
    vec4 Ka;
    vec4 Kd;
    vec4 Ks;
    float Ns;
};
uniform Material material;

uniform int useTexture = 0;
uniform int useNormalMap = 0;
uniform sampler2D diffuseColorSampler;
uniform sampler2D specularColorSampler;
uniform sampler2D normalMapSampler;

uniform samplerCube depthMap;

// output data
out vec4 fragmentColor;

vec4 phong(float visibility);
float shadowCalculation(vec3 fragPositionLightspace);

vec4 Kd, Ks, Ka;
float Ns;
vec3 normal;

const int samples = 20;
vec3 sampleOffsetDirections[samples] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

void main()
{   
    float visibility = 1 - shadowCalculation(fs_in.FragPos);
    fragmentColor = phong(visibility);
}

float shadowCalculation(vec3 fragPos){
    vec3 fragToLight = fragPos - light.lightPos;
    float closestDepth = texture(depthMap, fragToLight).r * light.farPlane;

    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.075;
    float viewDistance = length(viewPos - fragPos);
    float diskRadius = (1.0 + (viewDistance / light.farPlane)) / 25.0;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= light.farPlane;   // undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);
    return shadow;
}

vec4 phong(float visibility){
    if (useTexture == 1){
        Ka = vec4(0.05 * Kd.rgb, 1.0);
        Kd = texture(diffuseColorSampler, fs_in.TexCoords);
        Ks = texture(specularColorSampler, fs_in.TexCoords);
        Ns = 10;
    } else {
        Ks = material.Ks;
        Kd = material.Kd;
        Ka = material.Ka;
        Ns = material.Ns;
    }
    if (useNormalMap == 1){
        normal = normalize(texture(normalMapSampler, fs_in.TexCoords).rgb * 2.0 - 1.0);
        normal = normalize(fs_in.TBN * normal);
    } else {
        normal = fs_in.Normal;
    }
    normal = fs_in.Normal;

    vec4 Ia = Ka * light.La;

    vec3 lightDir = normalize(light.lightPos - fs_in.FragPos);
    vec4 Id = clamp(dot(lightDir, normal), 0, 1) * Kd * light.Ld;

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec4 Is = pow(max(dot(normal, halfwayDir), 0.0), Ns) * Ks * light.Ls;


    float constantAttenuation = 1.0;
    float linearAttenuation = 0.1; // Increase linear attenuation factor
    float quadraticAttenuation = 0.01;
    float lightDistance = length(light.lightPos - fs_in.FragPos);

    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * lightDistance + quadraticAttenuation * lightDistance * lightDistance);


    vec4 finalColor = Ia + (Id + Is) * visibility * attenuation * light.lightIntensity;
    finalColor.a = 1;
    return finalColor;
}

