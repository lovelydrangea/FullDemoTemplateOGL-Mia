#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "framework.h"
#include "DemoTemplateOGL.h"
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef UTILITIES_OGL_H
#define UTILITIES_OGL_H

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
#endif