#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset; 
layout (location = 2) in vec4 aColor; 
layout (location = 3) in float aRadius; 

uniform mat4 view; 
uniform mat4 projection; 


out vec2 position; 
out vec2 center; 
out vec4 color; 
out float radius; 

void main()
{ 
    float multiplier = aRadius; 

    gl_Position = projection * view * vec4(aPos*multiplier + aOffset, 0.0, 1.0);
    position = aPos*multiplier + aOffset; // (projection * view * vec4(aPosNew + aOffset, 0.0, 1.0)).xy; 
    
    radius = aRadius; 
    center = aOffset.xy; 
    color = aColor; 
}