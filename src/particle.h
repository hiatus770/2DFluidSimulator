#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class Particle 
{
public: 
    glm::vec2 pos; 
    glm::vec2 velocity;
    float mass = 1.0f; 
    float radius = 1.0f; 
    std::vector<float> color = {1.0f, 1.0f, 1.0f, 1.0f}; 

    Particle(glm::vec2 p, glm::vec2 v, std::vector<float> Color){
        pos = p; 
        velocity = v; 
        color = Color; 
    }; 
};

// Handles creating, distributing, global arraying, possibly rendering too 
class ParticleHandler 
{
public: 

    std::vector<float> quadPosition = {
        0.0f, 0.0f,
        100.0f, 0.0f, 
        100.0f, 100.0f, 
        0.0f, 100.0f
    };

    unsigned int VAO; 
    unsigned int VBO; 
    unsigned int positionVBO; 
    unsigned int colorVBO; 
    
    std::vector<Particle> particles;
    std::vector<float> positions; // Positions is generated each time from the particle global array just so we can feed it into the VBO  
    std::vector<float> colors; 

    int particleCount = 2;
    Shader* particleShader;  

    ParticleHandler(int ParticleCount){
        particleCount = ParticleCount; 
        particleShader = new Shader("/home/hiatus/Documents/2DFluidSimulator/src/shaders/circleVert.vs", "/home/hiatus/Documents/2DFluidSimulator/src/shaders/circleFrag.fs");
        
        // Temporary, jsut having one test particle right now 
        particles.push_back(Particle(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), {1.0f, 1.0f, 1.0f, 1.0f})); 
        particles.push_back(Particle(glm::vec2(300.0f, 300.0f), glm::vec2(0.0f, 0.0f), {1.0f, 1.0f, 1.0f, 1.0f})); 

        generateColorArray(); 
        generatePositionArray(); 

        glGenBuffers(1, &VBO); 
        glGenBuffers(1, &colorVBO); 
        
        glGenVertexArrays(1, &VAO); 

        glBindVertexArray(VAO); 
        // Quad to be used for drawing


        // POSITION VBO 
        glGenBuffers(1, &positionVBO);  
        glBindBuffer(GL_ARRAY_BUFFER, positionVBO); 
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), positions.data(), GL_STATIC_READ); 
        glEnableVertexAttribArray(1); 
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); 
        glVertexAttribDivisor(1, 1); 

        // Regular VBO 
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quadPosition.size(), quadPosition.data(), GL_STATIC_READ); 
        glEnableVertexAttribArray(0); 
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); 

        // Setting up each particle color 
        glEnableVertexAttribArray(2); 
        glBindBuffer(GL_ARRAY_BUFFER, colorVBO); 
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * colors.size(), colors.data(), GL_STATIC_READ); 
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); 
        glVertexAttribDivisor(2, 1); 


        // ONce we are done using we bind to 0 
        glBindBuffer(GL_ARRAY_BUFFER, 0); 

    };

    void render(GLenum mode = GL_LINES){
        particleShader->use();
        // particleShader->setVec3("cameraPos", {camera.position.x, camera.position.y, camera.position.z}); 
        particleShader->setMat4("view", camera.getViewMatrix());
        particleShader->setMat4("projection", glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f)); 
        
        glBindVertexArray(VAO); 
        glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, quadPosition.size(), particles.size()); 
        glBindVertexArray(0); 

    }

    void generateColorArray(){
        colors.clear(); 
        for(int i = 0; i < particles.size(); i++){
            colors.push_back(particles[i].color[0]);
            colors.push_back(particles[i].color[1]);
            colors.push_back(particles[i].color[2]);
            colors.push_back(particles[i].color[3]);
        }
    }

    void generatePositionArray(){
        positions.clear(); 
        for(int i = 0; i < particles.size(); i++){
            positions.push_back(particles[i].pos.x); 
            positions.push_back(particles[i].pos.y);
        }
    }


};



#endif