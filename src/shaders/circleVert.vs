#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aOffset; 
layout (location = 2) in vec4 aColor; 
 
uniform mat4 view; 
uniform mat4 projection; 


out vec2 position; 
out vec2 center; 
out vec4 color; 

void main()
{ 
    // gl_Position = projection * view * vec4(aPos + aOffset, 0.0, 1.0);
    gl_Position = projection * view * vec4(aPos + aOffset, 0.0, 1.0);
    position = (projection * view * vec4(aPos + aOffset, 0.0, 1.0)).xy; 
    center = aPos.xy + vec2(-50, -50); 
    color = aColor; 
}