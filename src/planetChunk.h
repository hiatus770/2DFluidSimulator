#ifndef PLANET_CHUNK
#define PLANET_CHUNK

#include "planet.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class PlanetChunk
{
public:
    glm::vec3 chunkPosition;
    // Data used to describe the position and orientation of each planet
    std::vector<glm::vec3> planetPositions;
    std::vector<glm::vec3> planetRotAxis;

    // Hold a vector of planets :sunglasses:
    std::vector<Planet> planetList;
    // Planet planetList[PLANETS_PER_CHUNK];

    Shader *planetShader;

    PlanetChunk(Shader *gShader, glm::vec3 ChunkPosition)
    {
        chunkPosition = ChunkPosition;
        planetShader = gShader;

        generatePlanetData(); // Generates the data for each planet
    }

    void generatePlanetData()
    {
        planetPositions.clear();
        planetRotAxis.clear();

        srand(10000 * (int)chunkPosition.x + 1000 * (int)chunkPosition.y + 100 * chunkPosition.z);

        for (int i = 0; i < PLANETS_PER_CHUNK; i++)
        {
            glm::vec3 position;
            glm::vec3 rotAxis;
            // Randomly generate some axes for rotation
            rotAxis.x = (rand() % 100);
            rotAxis.y = (rand() % 100);
            rotAxis.z = (rand() % 100);
            rotAxis = glm::normalize(rotAxis);

            // Now generate coordinates inside of the area defined by the chunk coordinates
            position.x = (float)(rand() % (CHUNK_SIZE * 100)) / 100.0f;
            position.y = (float)(rand() % (CHUNK_SIZE * 100)) / 100;
            position.z = (float)(rand() % (CHUNK_SIZE * 100)) / 100;

            float red = fabs(sin(rand() % 100));
            float green = fabs(sin(rand() % 100));
            float blue = fabs(sin(rand() % 100));

            planetPositions.push_back(glm::vec3(position.x + chunkPosition.x*CHUNK_SIZE, position.y + CHUNK_SIZE*chunkPosition.y, position.z + CHUNK_SIZE*chunkPosition.z));
            planetRotAxis.push_back(rotAxis);
            Planet tempPlanet(planetShader, planetPositions[i], rotAxis, {red, green, blue, 1.0f}); 
            planetList.push_back(tempPlanet); 
        }
    }


    void regenerate(){

    }

    void render(float deltaTime)
    {
        for(int i = 0; i < planetList.size(); i++){
            planetList[i].render(deltaTime, camera.getViewMatrix(), camera.getProjectionMatrix()); 
        }
    }
};

#endif
