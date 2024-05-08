#ifndef DRAWING
#define DRAWING 

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::vector<float> circleVertices = {}; 

// Global positions 
std::vector<std::pair<float, float>> positions; 


void generateCircleSprite(int iterations){
    circleVertices.push_back(0);
    circleVertices.push_back(0);  
    for (int i = 0; i < iterations; i++){
        float angle = (i/iterations) * 2 * M_PI; 
        // Knowing that we have a radius of one we simply calculate the sin x and cos x to find the coordinates
        circleVertices.push_back(cosf(angle)); 
        circleVertices.push_back(sinf(angle)); 
    }
}; 

void drawCircle(){
        
}

void renderCircles(){
    
    for (int i = 0; i < positions.size(); i++){
        
    }
}

#endif