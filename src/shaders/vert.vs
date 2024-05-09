#version 330 core
layout (location = 0) in vec2 aPos;

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection; 
uniform vec3 cameraPos; 

out float dist; 

void main()
{ 
    vec3 pos = vec3(projection * view * model * vec4(aPos.x, aPos.y, 0.0, 1.0)); 
    dist = length(pos); 
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, 0.0, 1.0);
    gl_Position = projection * vec4(aPos.x, aPos.y, 0.0, 1.0);
}