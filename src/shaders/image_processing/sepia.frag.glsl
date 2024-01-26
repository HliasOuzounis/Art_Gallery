#version 330 core
  
in vec2 vertexUV;

uniform sampler2D screenTexture;
uniform sampler2D pointsTexture;

out vec4 fragmentColor;

void main()
{
	vec4 originalColor = texture(screenTexture, vertexUV);

    float sepiaR = (originalColor.r * 0.393) + (originalColor.g * 0.769) + (originalColor.b * 0.189);
    float sepiaG = (originalColor.r * 0.349) + (originalColor.g * 0.686) + (originalColor.b * 0.168);
    float sepiaB = (originalColor.r * 0.272) + (originalColor.g * 0.534) + (originalColor.b * 0.131);

    fragmentColor = vec4(sepiaR, sepiaG, sepiaB, originalColor.a);
}