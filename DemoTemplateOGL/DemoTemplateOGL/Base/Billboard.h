#ifndef _billb
#define _billb
#include "Utilities.h"
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glext.h"
#include "wglext.h"

class Billboard {

private:
	/* data */
	Shader* shader;
	bool defaultShader;
	unsigned int diffuseMap;
	unsigned int BillboardVBO, BillboardVAO, BillboardEBO;
	glm::vec3 position, origin;
	float scale, rotationAngle, alto, ancho;
	float vertices[20];

	void reloadData(float* vertices){
		float verts[] = {
			// positions        // texture coords
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, //bottom left
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  //bottom right
			0.5f, 0.5f, 0.0f, 0.0f, 0.0f,   //top right
			-0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  //top left
		};

		for (int i = 0; i < 20; i++) {
			*(vertices + i) = *(verts + i);
		}
	}

	void reloadData(float* vertices, float width, float height, float x, float y){

		glm::vec3 billcam = glm::vec3(cameraDetails->getPosition().x - origin.x,
			cameraDetails->getPosition().y - origin.y,
			cameraDetails->getPosition().z - origin.z);

		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 right = glm::normalize(glm::cross(up, billcam));
		right.x *= this->ancho / 2.0;
		right.z *= this->ancho / 2.0;

		float tc[4][2] = {
			{1, 1}, // top right
			{0, 1},       // top left

			{0, 0},             // bottom left
			{1, 0},       // bottom right
		};

		float verts[] = {
			// positions        // texture coords
			origin.x + right.x, origin.y, origin.z + right.z, tc[0][0], tc[0][1],          //bottom left
			origin.x - right.x, origin.y, origin.z - right.z, tc[1][0], tc[1][1],          //bottom right

			origin.x - right.x, origin.y + alto, origin.z - right.z, tc[2][0], tc[2][1], //top right
			origin.x + right.x, origin.y + alto, origin.z + right.z, tc[3][0], tc[3][1], //top left
		};

		for (int i = 0; i < 20; i++)
		{
			*(vertices + i) = *(verts + i);
		}
	}
public:
	Camera* cameraDetails = NULL;
	Billboard(WCHAR textura[], float ancho, float alto, float x, float y, float z, Camera* camera) {
		cameraDetails = camera;
		int mapAlturaX, mapAlturaY, mapAlturaComp;
		this->alto = alto;
		this->ancho= ancho;
//		this->ancho = ancho * 2;
//		this->alto = alto * 2;

		origin = glm::vec3(x, y, z);
		wstring tex(textura);
		string text(tex.begin(), tex.end());
		unsigned char* mapaAlturas = loadFile(text.c_str(), &mapAlturaX, &mapAlturaY, &mapAlturaComp, 0);
		GLenum format = GL_RGBA;
		if (mapAlturaComp == 1)
			format = GL_RED;
		else if (mapAlturaComp == 3)
			format = GL_RGB;
		else if (mapAlturaComp == 4)
			format = GL_RGBA;

		reloadData(vertices);
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		glGenVertexArrays(1, &BillboardVAO);
		glGenBuffers(1, &BillboardVBO);
		glGenBuffers(1, &BillboardEBO);

		glBindVertexArray(BillboardVAO);

		glBindBuffer(GL_ARRAY_BUFFER, BillboardVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BillboardEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenTextures(1, &diffuseMap);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// load image, create texture and generate mipmaps
		glTexImage2D(GL_TEXTURE_2D, 0, format, mapAlturaX, mapAlturaY, 0, format, GL_UNSIGNED_BYTE, mapaAlturas);
		glGenerateMipmap(GL_TEXTURE_2D);
		// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
		// -------------------------------------------------------------------------------------------
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		delete []mapaAlturas;
		defaultShader = false;
	}

	~Billboard(){
		if (shader != NULL) {
			delete shader;
			shader = NULL;
		}
		glDeleteTextures(1, &diffuseMap);
	}

	// Usa el shader default para poder imprimir el billboard
	void Draw() {
		if (shader == NULL) {
			// build and compile our shader zprogram
			// ------------------------------------
			shader = new Shader("shaders/billboard.vs", "shaders/billboard.fs");
			defaultShader = true;
		}
		if (defaultShader) {
			shader->use();
			prepShader(*shader);
			shader->setInt("texture1", 0);
			Draw(*shader);
			shader->desuse();
		} else Draw(*shader);
	}

	void Draw(Shader &shader) {
		//habilitamos el culling para reducir tiempo de procesamiento de las texturas
		//las texturas se analizan pixel a pixel para determinar que se imprimen o no
		//por lo que aunque no la veamos nos costo, por eso la eliminamos.
		glEnable(GL_CULL_FACE);
		//hay dos tipos de cull el frontal y el trasero (back)
		glCullFace(GL_FRONT);

		glEnable(GL_BLEND);
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		// render container
		glBindVertexArray(BillboardVAO);
		reloadData(vertices, 1, 1, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, BillboardVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDisable(GL_BLEND);
	}

	void prepShader(Shader& shader) {
		glm::mat4 projection = cameraDetails->getProjection();
		glm::mat4 view = cameraDetails->getView();

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f)); // translate it down so it's at the center of the scene
//		model = glm::scale(model, glm::vec3(3.0f,3.0f,3.0f));

		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setMat4("model", model);
	}
};

#endif 