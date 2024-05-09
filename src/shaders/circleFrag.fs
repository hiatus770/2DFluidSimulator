#version 330 core
out vec4 FragColor;
  
in vec2 coordinate; 
in vec4 color; 
in vec2 position;
in vec2 center;  
in float radius; 

void main()
{

    FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f); 
    if (length(position - center) <= 50){
        FragColor = vec4(color.xyz, 1.0f);
    }
}
