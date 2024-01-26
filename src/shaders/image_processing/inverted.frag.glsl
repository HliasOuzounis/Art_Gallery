#version 330 core
  
in vec2 vertexUV;

uniform sampler2D screenTexture;

out vec4 fragmentColor;

void main()
{
	fragmentColor = 1 - vec4(texture(screenTexture, vertexUV).rgb, 1.0);
}