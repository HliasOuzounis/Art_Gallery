#version 330 core

in vec2 vertexUV;

uniform sampler2D screenTexture;

out vec4 fragmentColor;

int brushSize[3] = int[](8, 4, 2);

void main()
{
    // Grayscale conversion
    vec3 diffuse = texture(screenTexture, vertexUV.st).rgb;
    vec3 color[3] = vec3[](vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0));

    for (int k; k < 3; k++){
        for (int i; i < brushSize[k]; i++){
            for (int j; j < brushSize[k]; j++){
                color[k] += texture(screenTexture, vertexUV.st + vec2(i, j) / 512).rgb;
            }
        }
        // color[k] /= float(brushSize[k] * brushSize[k]);
    }

    fragmentColor = vec4(color[0] * 0.1 + color[1] * 0.2 + color[2] * 0.3 + diffuse * 0.4, 1.0);  
    
}