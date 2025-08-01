#version 430 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex; 

void main()
{
    vec3 color = texture(tex, TexCoords).rgb;

    // Display the SSAO as a grayscale value
    FragColor = vec4(color, 1.0);
}
