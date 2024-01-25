#version 330 core
  
in vec2 vertexUV;

uniform sampler2D screenTexture;
uniform sampler2D pointsTexture;

out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(texture(screenTexture, vertexUV).rgb, 1.0) * (1 - vec4(texture(pointsTexture, vertexUV).rgb, 1.0));
}