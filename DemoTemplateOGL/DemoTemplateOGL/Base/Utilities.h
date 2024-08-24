#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <time.h>
#include <map>
#include <vector>
#include "../WinAPIHeaders/framework.h"
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
// FLAG TO DISPLAY ERRORS ON MessageBox
#define SHOWLOGGERMB

#define UTILITIES_OGL_H
// ejemplo de uso de enumeracion para definir cual eje se utilizara
enum Ejes { EjeX, EjeY, EjeZ };

// Tamanio de la ventana
extern long get_nanos();
extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

#ifdef _WIN32 
extern struct GameTime {
#else
struct GameTime {
#endif
	long lastTick = 0;
	long deltaTime = 0;
	GameTime(){
	}
};
extern GameTime gameTime;

#ifdef _WIN32 
extern struct GameActions {
#else
struct GameActions {
#endif
	// float /// x = 0 -> quiet; x > 0 -> move positive; x < 0 -> move negative
	float advance = 0;
	float hAdvance = 0;
	float sideAdvance = 0;
	bool firstPerson = false;
	bool jump = false;
	bool action = false;
	void setZoom(float value);
	float* getZoom();
	void setPlayerZoom(float value);
	float* getPlayerZoom();
	void setAngle(float value);
	float* getAngle();
	void setPitch(float value);
	float* getPitch();
	~GameActions();
	private:
		float* zoom = NULL;
		float* playerZoom = NULL;
		float* angle = NULL;
		float* pitch = NULL;
};

// Estructura Vertice que contiene las propiedades del mesh
#ifdef _WIN32 
extern struct Vertex {
#else
struct Vertex {
#endif
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
#ifdef _WIN32 
extern struct Texture {
#else
struct Texture {
#endif
	unsigned int id;
	std::string type;
	std::string path;
};

extern std::wstring s2ws(const std::string& s);

namespace UTILITIES_OGL {

#ifdef _WIN32 
	extern struct ImageDetails {
#else
	struct ImageDetails {
#endif
		int width;
		int height;
		int nrComponents;
		GLenum format;
	};

	//estructura para manejar primitivos con posicion, normal y uv's
#ifdef _WIN32 
	extern struct Vertices {
#else
	struct Vertices {
#endif
		float Posx, Posy, Posz;
		float Normx, Normy, Normz;
		float u, v;
	};

	//En honor a nuestros ancestros llamaremos "Maya" a la malla
	//estructura que contiene datos de los vertices y sus indices
#ifdef _WIN32 
	extern struct Maya {
#else
	struct Maya {
#endif
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
extern unsigned int TextureFromFile(const char* path, const std::string& directory, bool rotateX = false, bool rotateY = true, bool *alpha = NULL, struct UTILITIES_OGL::ImageDetails* img = NULL);

#pragma once
#ifndef LOGGER_H
#define LOGGER_H

namespace LOGGER {
	class LOG {
	private:
		void* WINDOW;
		std::string name;
	public:
		LOG();
		LOG(std::string filename);
		LOG(std::string filename, void* hwnd);
		std::string getLogger();
		void info(const std::string log, const std::string title);
		void info(const std::string log);
		void info(const char* log);
		void info(const char* log, const char *title);
		void setWindow(void* hwnd);
		void* getWindow();
	};

	class LOGS {
	private:
		static std::vector<LOG> log;
	public:
		static void* WINDOW;
		static LOG getLOGGER();
		static LOG getLOGGER(std::string filename);
	};
}
#endif

#endif