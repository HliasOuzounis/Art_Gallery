#version 330 core

in vec2 vertexUV;

uniform sampler2D screenTexture;

out vec4 fragmentColor;

mat3 sx = mat3( 
    1.0, 2.0, 1.0, 
    0.0, 0.0, 0.0, 
   -1.0, -2.0, -1.0 
);
mat3 sy = mat3( 
    1.0, 0.0, -1.0, 
    2.0, 0.0, -2.0, 
    1.0, 0.0, -1.0 
);

float[] levels = float[](0.0, 0.1, 0.3, 0.6, 1.0);


float quantize(float color){
    float quantizedColor = 0.0;
    for (int i = 0; i < levels.length; i++) {
        if (color >= levels[i]) {
            quantizedColor = levels[i];
        }
    }
    return quantizedColor;
}

void main()
{
    // Grayscale conversion
    vec3 diffuse = texture(screenTexture, vertexUV.st).rgb;
    float gray = 0.299 * diffuse.r + 0.587 * diffuse.g + 0.114 * diffuse.b;
    diffuse = vec3(gray);

    // Sobel filter
    mat3 I;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vec3 sample = texelFetch(screenTexture, ivec2(gl_FragCoord) + ivec2(i - 1, j - 1), 0).rgb;
            I[i][j] = length(sample);
        }
    }

    float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]);
    float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);

    float g = sqrt(pow(gx, 2.0) + pow(gy, 2.0));

    // Reduce noise
    g = smoothstep(0.3, 0.7, g);

    vec3 edgeColor = vec3(0., 0., 0.);
    vec3 resultColor = mix(diffuse, edgeColor, g);

    // Apply quantization
    resultColor = vec3(quantize(resultColor.r), quantize(resultColor.g), quantize(resultColor.b));

    fragmentColor = vec4(resultColor, 1.0);
}