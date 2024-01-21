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

void main()
{
	// Sobel filter
    vec3 diffuse = texture(screenTexture, vertexUV.st).rgb;
    mat3 I;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            vec3 sample  = texelFetch(screenTexture, ivec2(gl_FragCoord) + ivec2(i-1,j-1), 0 ).rgb;
            I[i][j] = length(sample); 
		}
	}

	float gx = dot(sx[0], I[0]) + dot(sx[1], I[1]) + dot(sx[2], I[2]); 
	float gy = dot(sy[0], I[0]) + dot(sy[1], I[1]) + dot(sy[2], I[2]);

	float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));

	// Reduce noise
    g = smoothstep(0.5, 0.7, g);

    vec3 edgeColor = vec3(0., 0., 0.);
    fragmentColor = vec4(mix(diffuse, edgeColor, g), 1.);

	int quantization = 25;
	fragmentColor = vec4(round(fragmentColor.rgb * quantization) / quantization, 1.0);
} 