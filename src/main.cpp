#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <utility>
#include "shader.h"
#include "object.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture.h"

const int SRC_WIDTH = 1920;
const int SRC_HEIGHT = 1080;
const int CHUNK_SIZE = 1;
const int STARS_PER_CHUNK = 100;  
const int PLANETS_PER_CHUNK = 10;

#include "camera.h"

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f)); // Global Camera for the entire code thing :)

#include "player.h"
#include "compute.h"
#include "particle.h"

// Whenever the window is changed this function is called
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
// Mouse call back
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
// Processing all input here
void processInput(GLFWwindow *window);

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;



int main()
{
    std::cout << "Making Window!" << std::endl;

    // ------------ OPENGL INTIALIZATION ----------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                 // The major version so x.x the first x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                 // The minor version of GLFW we are using so x.x the second x
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // This means we do not use any backwards compatible features, so its a smaller set of all of OPENGL

    // Creating the window object
    GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Balls!", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW Window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);

    // Load GLAD function pointers so that we use the correct openGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // OPENGL INITIALIZATION
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glEnable(GL_DEPTH_TEST);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Temporary code before i place it into a class
    ComputeShader computeShader("/home/hiatus/Documents/2DFluidSimulator/src/shaders/compute.vs"); 
    
    int workGroupSize = 10; 
    int xAmt = 100; 
    int yAmt = 100;
    float deltaL = 5.0f; 

    // computeShader.setFloat("delta", deltaL); 

    // Important vectors to track 
    std::vector<float> positions; 
    std::vector<float> outputPositions; 
    std::vector<float> metaballs = {50.01f, 50.03f, 30.0f, 0.0f}; // These just contain the x and y coordinate of the center along with the scaling factor! 
    for(int i = 0; i < xAmt; i++){
        for(int j = 0; j < yAmt; j++){
            positions.push_back(i * deltaL);
            positions.push_back(j * deltaL);
        }
    }
    
    unsigned int positionSSBO; 
    glGenBuffers(1, &positionSSBO); 
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, positionSSBO);  
    glBufferData(GL_SHADER_STORAGE_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);   
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, positionSSBO); 
    
    // Metaballs SSBO 
    unsigned int metaballsSSBO;  
    glGenBuffers(1, &metaballsSSBO); 
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, metaballsSSBO);  
    glBufferData(GL_SHADER_STORAGE_BUFFER, metaballs.size() * sizeof(float), metaballs.data(), GL_STATIC_READ); 
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, metaballsSSBO);  

    unsigned int outputPositionSSBO; 
    glGenBuffers(1, &outputPositionSSBO); 
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputPositionSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, positions.size() * 4 * sizeof(float), NULL, GL_STATIC_DRAW); 
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, outputPositionSSBO); 

    Shader globalShader("/home/hiatus/Documents/2DFluidSimulator/src/shaders/vert.vs", "/home/hiatus/Documents/2DFluidSimulator/src/shaders/frag.fs");

    //Object testObject(&globalShader, {0.0f, 0.0f, 1080.0f, 0.0f, 1920.0f, 1080.0f}); 
    Object testObject(&globalShader, {0.0f, 0.0f, 1080.0f/2, 0.0f, 1920.0f/2, 1080.0f/2}); 
 
    // Main Loop of the function
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        std::cout << "FPS: " << 1 / deltaTime << std::endl;

        // Clear the screen before we start
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process input call
        processInput(window);

        // Running compute shader
        computeShader.use(); 
        computeShader.dispatch(); 
        computeShader.wait();         

        // OLD RENDERING CODE!
        // GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; 
        // glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputPositionSSBO);
        // float output[positions.size()*4];
        // glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 4 * positions.size() * sizeof(float), &output); 
        
        
        // for(int i = 0; i < positions.size() * 4 -1; i+=2){
        //     // std::cout << output[i] << " " << output[i+1] << " | "; 
        //     outputPositions.push_back(output[i]); 
        //     outputPositions.push_back(output[i+1]); 
        // } // std::cout << std::endl; 

        // break; 

        // // glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        // testObject.vertices = outputPositions;   
        
        // testObject.render(camera.getViewMatrix(), camera.getProjectionMatrix(),  GL_LINES); 
       
        // NEW RENDERING CODE!
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, outputPositionSSBO); 
        testObject.shader->use(); 
        testObject.shader->setVec4("color", testObject.objColor); 
        testObject.shader->setMat4("model", testObject.model); 
        testObject.shader->setMat4("view", camera.getViewMatrix()); 
        testObject.shader->setMat4("projection", camera.getProjectionMatrix()); 
        glBindVertexArray(testObject.VAO);
        glDrawArraysInstanced(GL_LINES, 0, 2, 100*100);  
        // glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 3); 
        
        // glDrawArraysInstanced(GL_LINES, 0, positions.size()/2, outputPositions.size()/2); 

        glfwSwapBuffers(window); // Swaps the color buffer that is used to render to during this render iteration and show it ot the output screen
        glfwPollEvents();        // Checks if any events are triggered, updates the window state andcalls the corresponding functions
    }

    glfwTerminate(); // Call this function to properly clean up GLFW's big mess
    return 0;
}

// This function is going to be used to resize the viewport everytime it is resized by the user
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Currently no mouse controls are enabled 
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    // float xpos = static_cast<float>(xposIn);
    // float ypos = static_cast<float>(yposIn);
    // if (firstMouse)
    // {
    //     lastX = xpos;
    //     lastY = ypos;
    //     firstMouse = false;
    // }

    // float xoffset = xpos - lastX;
    // float yoffset = lastY - ypos;
    // lastX = xpos;
    // lastY = ypos;

    // float sensitivity = 0.1f;
    // xoffset *= sensitivity;
    // yoffset *= sensitivity;

    // yaw   += xoffset;
    // pitch += yoffset;

    // if(pitch > 89.0f)
    //     pitch = 89.0f;
    // if(pitch < -89.0f)
    //     pitch = -89.0f;

    // glm::vec3 direction;
    // direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    // direction.y = sin(glm::radians(pitch));
    // direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    // cameraFront = glm::normalize(direction);
}

/**
 * @brief Handles all user input given the window object, currently handles player movement 
 * 
 * @param window 
 */
void processInput(GLFWwindow *window)
{
    // // Function is used as follows player.processKeyboard(ENUM, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     player.processKeyboard(FORWARD, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     player.processKeyboard(BACKWARD, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     player.processKeyboard(STRAFE_LEFT, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     player.processKeyboard(STRAFE_RIGHT, deltaTime);
    
    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    //     player.processKeyboard(PITCH_UP, deltaTime);
    // else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //     player.processKeyboard(PITCH_DOWN, deltaTime);
    
    // if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    //     player.processKeyboard(YAW_RIGHT, deltaTime);
    // else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    //     player.processKeyboard(YAW_LEFT, deltaTime);
    
    // if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    //     player.processKeyboard(RISE, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    //     player.processKeyboard(FALL, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    //     player.processKeyboard(ROLL_RIGHT, deltaTime);
    // if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    //     player.processKeyboard(ROLL_LEFT, deltaTime);

    // if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    // {
    //     /// Reset orientation
    //     player.direction = glm::vec3(1.0f, 0.0f, 0.0f);
    //     player.localUp = glm::vec3(0.0f, 1.0f, 0.0f);
    // }

    // // Update calls
    // player.update(); 

    // std::cout << player.MovementSpeed << std::endl; 
    
    // camera.position = player.getCameraPosition();
    // camera.direction = player.getCameraDirection();
    // camera.cameraUp = player.getCameraUp();
    // camera.projection =  glm::perspective(glm::radians(60.0f), (float)SRC_WIDTH/SRC_HEIGHT, 0.1f, 1000.0f);     
}
