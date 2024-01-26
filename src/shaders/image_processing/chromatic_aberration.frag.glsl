#version 330 core
  
in vec2 vertexUV;

uniform sampler2D screenTexture;
uniform float time;

out vec4 fragmentColor;

float periodicFunction(float time) {
    // Adjust the frequency and amplitude as needed
    float frequency = 0.2;
    float amplitude = 0.03;

    // Smooth oscillation using sine function
    float smoothOscillation = amplitude * sin(2.0 * 3.14159265358979323846 * frequency * time);

    // Exponential term for occasional spikes
    float exponentialTerm = exp(-0.1 * cos(2.0 * 3.14159265358979323846 * 0.1 * time));

    // Combine smooth oscillation and spikes
    return smoothOscillation * exponentialTerm;
}

void main()
{
    float aberrationAmount = 0.03; // Adjust as needed

    vec3 color;

    // Convert time to radians
    float timeInRadians = time * 2.0 * 3.14159265358979323846 * 0.01;

    // Apply chromatic aberration
    color.r = texture(screenTexture, vertexUV + vec2(aberrationAmount + periodicFunction(time), periodicFunction(-time * 2))).r;
    color.g = texture(screenTexture, vertexUV + vec2(periodicFunction(time), periodicFunction(time))).g;
    color.b = texture(screenTexture, vertexUV - vec2(aberrationAmount + periodicFunction(-time * 1.5), periodicFunction(time))).b;

    fragmentColor = vec4(color, 1.0);
}
