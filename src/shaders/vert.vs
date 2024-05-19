#version 430 core
layout (location = 0) in vec2 aPos;

layout(binding = 7, std430) buffer outputpositions {
    vec2 outputPositions[];
};

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection; 
uniform vec3 cameraPos; 


void main()
{ 
    vec2 bPos = outputPositions[0 + gl_InstanceID/100]; 
    // vec3 pos = vec3(projection * view * model * vec4(aPos.x, aPos.y, 0.0, 1.0)); 
    // gl_Position = projection * view * model * vec4(aPos.x, aPos.y, 0.0, 1.0);
    gl_Position = projection * view * vec4(aPos.x, bPos.y, 0.0, 1.0);
}