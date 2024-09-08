#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;
out vec3 WorldPos;
//out vec4 FragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//uniform mat4 lightSpaceMatrix;

uniform vec3 scale;        
uniform vec3 tilingFactor;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    WorldPos = worldPos.xyz;
    FragPos = worldPos.xyz; 
    //TexCoords = aTexCoords * (tilingFactor.xy / scale.xy);
    TexCoords = aTexCoords;
    
    Normal = transpose(inverse(mat3(model))) * aNormal;  
    //FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    gl_Position = projection * view * worldPos;
}