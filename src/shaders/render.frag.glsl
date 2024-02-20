#version 330 core

in VS_OUT{   
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
    vec3 tangentFragPos;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
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
uniform int useDisplacementMap = 0;
uniform sampler2D diffuseColorSampler;
uniform sampler2D specularColorSampler;
uniform sampler2D normalMapSampler;
uniform sampler2D displacementMapSampler;

uniform samplerCube depthMap;

// output data
out vec4 fragmentColor;

vec4 phong(float visibility);
float shadowCalculation(vec3 fragPositionLightspace);

vec2 texCoords;
vec2 parallaxMapping(vec2 texCoords, vec3 viewDir);

vec4 Kd, Ks, Ka;
float Ns;

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
    texCoords = (useDisplacementMap == 0) ? fs_in.TexCoords :
                parallaxMapping(fs_in.TexCoords, normalize(fs_in.tangentViewPos - fs_in.tangentFragPos));

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
        Kd = texture(diffuseColorSampler, texCoords);
        Ks = texture(specularColorSampler, texCoords);
        Ns = 2;
    } else {
        Ka = material.Ka;
        Kd = material.Kd;
        Ks = material.Ks;
        Ns = material.Ns;
    }
    
    vec3 normal = (useNormalMap == 1) ? normalize(texture(normalMapSampler, texCoords).rgb * 2.0 - 1.0) : fs_in.Normal;

    vec4 Ia = Ka * light.La;

    vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentFragPos);
    vec4 Id = clamp(dot(lightDir, normal), 0, 1) * Kd * light.Ld;

    vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec4 Is = pow(max(dot(normal, halfwayDir), 0.0), Ns) * Ks * light.Ls;


    float constantAttenuation = 1.0;
    float linearAttenuation = 0.1; // Increase linear attenuation factor
    float quadraticAttenuation = 0.01;
    float lightDistance = length(fs_in.tangentLightPos - fs_in.tangentFragPos);

    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * lightDistance + quadraticAttenuation * lightDistance * lightDistance);

    vec4 finalColor = Ia + (Id + Is) * visibility * attenuation * light.lightIntensity;
    finalColor.a = 1;
    return finalColor;
}

vec2 parallaxMapping(vec2 startingTexCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));  

    const float height_scale = 0.1;
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * height_scale; 
    vec2 deltaTexCoords = P / numLayers;    

    vec2  currentTexCoords     = startingTexCoords;
    float currentDepthMapValue = texture(displacementMapSampler, currentTexCoords).r;
    
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(displacementMapSampler, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(displacementMapSampler, prevTexCoords).r - currentLayerDepth + layerDepth;
    
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords; 
} 
