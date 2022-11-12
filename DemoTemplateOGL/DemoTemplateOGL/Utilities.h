#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "framework.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glext.h"
#include "wglext.h"
#include <FreeImage.h>

#ifndef UTILITIES_OGL_H
#define UTILITIES_OGL_H
// ejemplo de uso de enumeracion para definir cual eje se utilizara
enum Ejes { EjeX, EjeY, EjeZ };

// Tamanio de la ventana
extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

// Estructura Vertice que contiene las propiedades del mesh
extern struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
	Vertex();
	Vertex(glm::vec3 pos, glm::vec2 texCoord, glm::vec3 normal, glm::vec3 color);
};

// Estructura de Texture para guardar el ID de la textura y su tipo
extern struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

extern std::wstring s2ws(const std::string& s);

namespace UTILITIES_OGL {

	extern struct ImageDetails {
		int width;
		int height;
		int nrComponents;
		GLenum format;
	};

	//estructura para manejar primitivos con posicion, normal y uv's
	extern struct Vertices {
		float Posx, Posy, Posz;
		float Normx, Normy, Normz;
		float u, v;
	};

	//En honor a nuestros ancestros llamaremos "Maya" a la malla
	//estructura que contiene datos de los vertices y sus indices
	extern struct Maya {
		Vertices* maya;
		unsigned int* indices;
	};

	extern void sumaNormal(float* v1, float* v2);
	extern void normaliza(float* v1);
	extern void vectoresEsfera(Maya esfera, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, int iv, int ii);
	extern Maya Esfera(int stacks, int slices, float radio, float inicio, float final);
	extern Maya Plano(int vertx, int vertz, float anchof, float profz);
	extern Maya Plano(int vertx, int vertz, float anchof, float profz, unsigned char* altura, int nrComponents = 4, float tile = 1);
	extern glm::vec3 genNormal(float* v1, float* v2, float* v3);
}
extern unsigned char* loadFile(char const* filename, int* x, int* y, int* comp, int req_comp, bool rotateX = false, bool rotateY = true);
extern unsigned int TextureFromFile(HWND hWnd, const char* path, const std::string& directory, bool rotateX = false, bool rotateY = true, bool alpha = false, struct UTILITIES_OGL::ImageDetails* img = NULL);

#endif