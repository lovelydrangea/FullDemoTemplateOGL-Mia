#pragma once
#ifndef _wata
#define _wata
#include "Base/Utilities.h"
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Base/glext.h"
#include "Base/wglext.h"
#include "Terreno.h"

class Water : public Terreno {

private:
    void reloadData(vector<Vertex>& vertices) {
        // Increment time for animation
        static float waveSpeed = 0.1;
        static float time = 0;
        static float waveAmplitude = 0.5;
        static float waveFrequency = 0.5;
        time += waveSpeed;

        // Define the size of the water surface
        static const float water_size = 200;

        // Define the number of segments for the water surface
        int numSegmentsX = 400; // Number of segments in X direction
        int numSegmentsZ = 400; // Number of segments in Z direction

        for (int x = 0; x < numSegmentsX; x++) {
            for (int z = 0; z < numSegmentsZ; z++) {
                // Calculate position based on wave function
				Vertex &vertex = vertices.at((x * numSegmentsX) + z);
				float xPos = vertex.Position.x;
                float zPos = vertex.Position.z;
                float yPos = waveAmplitude * sin(waveFrequency * (xPos + time)) + waveAmplitude * sin(waveFrequency * (zPos + time));

                // Add vertex with updated position and texture coordinates
				vertex.Position.y = yPos;
//                vertex.TexCoords = glm::vec2((float)x / (numSegmentsX - 1), (float)z / (numSegmentsZ - 1));
            }
        }

    }
public:
	Water(WCHAR alturas[], WCHAR textura[], float ancho, float prof, Camera* camera)
		: Terreno(alturas, textura, ancho, prof, camera, GL_DYNAMIC_DRAW) {
		gpuDemo = NULL;
		buildKDtree();
	}

	~Water() {
		//nos aseguramos de disponer de los recursos previamente reservados
	}

	void Draw() {
		if (gpuDemo == NULL) {
			gpuDemo = new Shader("shaders/models/1.model_loading.vs", "shaders/models/1.model_loading.fs");
			setDefaultShader(true);
		}
		if (getDefaultShader()) {
			gpuDemo->use();
			prepShader(*gpuDemo);
			Draw(*gpuDemo);
			gpuDemo->desuse();
		}
		else Draw(*gpuDemo);
	}

	void Draw(Shader& shader) {
		reloadData(meshes[0].vertices);
		Model::Draw(shader);
	}

};

#endif 