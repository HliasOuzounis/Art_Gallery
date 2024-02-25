#version 330 core
  
in vec2 vertexUV;

uniform sampler2D screenTexture;
uniform sampler2D pointsTexture;

out vec4 fragmentColor;


vec2 center = vec2(0.5, 0.5);

void main()
{
    vec2 dist = vertexUV - center;
    float radius = length(dist);

    if (radius > 0.55)
    {
        fragmentColor = vec4(0, 0, 0, 1);
        return;
    }

    float distortion = pow(radius, 2.5) * 2.5;

    vec2 distortedUV = center + normalize(dist) * distortion;

    fragmentColor = texture(screenTexture, distortedUV);
}