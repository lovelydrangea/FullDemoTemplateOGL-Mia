#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Utilities.h"

// Global Variables:
struct Vertex;
struct Texture;

std::wstring s2ws(const std::string& s) {
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

struct UTILITIES_OGL::ImageDetails;
struct UTILITIES_OGL::Vertices;
struct UTILITIES_OGL::Maya;

//generamos las normales a traves de punteros del vector, es una forma comun de manejarlos
glm::vec3 UTILITIES_OGL::genNormal(float* v1, float* v2, float* v3) {
	glm::vec3 vec1, vec2;

	vec1.x = *v2 - *v1;
	vec1.y = *(v2 + 1) - *(v1 + 1);
	vec1.z = *(v2 + 2) - *(v1 + 2);

	vec2.x = *v3 - *v1;
	vec2.y = *(v3 + 1) - *(v1 + 1);
	vec2.z = *(v3 + 2) - *(v1 + 2);

	return glm::cross(vec1, vec2);
}

void UTILITIES_OGL::sumaNormal(float* v1, float* v2){
	*v1 += *v2;
	*(v1 + 1) += *(v2 + 1);
	*(v1 + 2) += *(v2 + 2);
}

void UTILITIES_OGL::normaliza(float* v1){
	float magnitud = sqrt((*v1) * (*v1) + (*(v1 + 1)) * (*(v1 + 1)) + (*(v1 + 2)) * (*(v1 + 2)));
	*v1 /= magnitud;
	*(v1 + 1) /= magnitud;
	*(v1 + 2) /= magnitud;
}

void UTILITIES_OGL::vectoresEsfera(Maya esfera, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, int iv, int ii) {
	for (int i = 0; i < iv || i < ii; i++) {
		if (i < iv) {
			Vertex v;
			v.Position.x = esfera.maya[i].Posx;
			v.Position.y = esfera.maya[i].Posy;
			v.Position.z = esfera.maya[i].Posz;
			v.Normal.x = esfera.maya[i].Normx;
			v.Normal.y = esfera.maya[i].Normy;
			v.Normal.z = esfera.maya[i].Normz;
			v.TexCoords.x = esfera.maya[i].u;
			v.TexCoords.y = esfera.maya[i].v;
			vertices.push_back(v);
		}
		if (i < ii)
			indices.push_back(esfera.indices[i]);

	}
}

//generamos los vertices a traves de coordenadas esfericas
//conocimiento adquirido en la materia de Fund de las Graficas Computacionales
UTILITIES_OGL::Maya UTILITIES_OGL::Esfera(int stacks, int slices, float radio, float inicio, float final) {
	//Cargamos la estructura con los espacios de memoria necesarios
	Vertices* verticesxyzSD = new Vertices[stacks * slices * 3];
	unsigned int* indices = new unsigned int[stacks * slices * 6];
	//generamos un objeto para poder transportar los punteros
	Maya salida;
	//a darle que es mole de olla!
	for (int i = 0; i < slices; i++)
	{
		for (int j = 0; j < stacks; j++)
		{
			int indice = (i * stacks + j);
			verticesxyzSD[indice].Posx = radio * cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
				cos(2.0 * M_PI * (double)i / (slices - 1));
			verticesxyzSD[indice].Posy = radio * sin(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0);
			verticesxyzSD[indice].Posz = radio * cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
				sin(2.0 * M_PI * (double)i / (slices - 1));

			verticesxyzSD[indice].Normx = cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
				cos(2.0 * M_PI * (double)i / (slices - 1));
			verticesxyzSD[indice].Normy = sin(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0);
			verticesxyzSD[indice].Normz = cos(((double)j / (stacks - 1)) * (M_PI * (final - inicio)) + M_PI * inicio - M_PI / 2.0) *
				sin(2.0 * M_PI * (double)i / (slices - 1));

			verticesxyzSD[indice].u = (float)1 * (1 - (float)i / (stacks - 1));
			verticesxyzSD[indice].v = (float)1 * (1 - (float)j / (slices - 1));
		}
	}

	//ahora la parte mas importante de crear vertices es el algoritmo para unirlos, en este caso sustituiremos
	//a un algoritmo con un un grupo de indices
	int indice = 0;
	for (int i = 0; i < slices - 1; i++)
	{
		for (int j = 0; j < stacks - 1; j++)
		{
			indices[indice++] = i * stacks + j;
			indices[indice++] = (i + 1) * stacks + j + 1;
			indices[indice++] = i * stacks + j + 1;

			indices[indice++] = i * stacks + j;
			indices[indice++] = (i + 1) * stacks + j;
			indices[indice++] = (i + 1) * stacks + j + 1;
		}
	}

	//una vez generados los damos a conocer a traves del objeto "salida"
	salida.maya = verticesxyzSD;
	salida.indices = indices;

	return salida;
}

UTILITIES_OGL::Maya UTILITIES_OGL::Plano(int vertx, int vertz, float anchof, float profz){
	//Cargamos la estructura con los espacios de memoria necesarios
	Vertices* verticesxyzSD = new Vertices[vertx * vertz * 3];
	unsigned int* indices = new unsigned int[vertx * vertz * 6];

	//es la separacion entre vertices, se le resta 1 para que el lado correcto
	//imagine que el ancho es de 10 y tiene 10 vertices, entonces le daria un deltax
	//de 1, si los vertices van de 0 a 9 entonces la posicion del ultimo vertice
	//seria 9, si le divide entre vertx -1 le dara 1.1111, y el ultimo vertice sera 10
	float deltax = anchof / (vertx - 1);
	float deltaz = profz / (vertz - 1);

	//crea los vertices
	for (int z = 0; z < vertz; z++)
	{
		for (int x = 0; x < vertx; x++)
		{
			verticesxyzSD[z * vertx + x].Posx = (float)x * deltax;
			verticesxyzSD[z * vertx + x].Posy = 0.0;
			verticesxyzSD[z * vertx + x].Posz = (float)z * deltaz;

			//carga las normales con cero
			verticesxyzSD[z * vertx + x].Normx = 0.0;
			verticesxyzSD[z * vertx + x].Normy = 0.0;
			verticesxyzSD[z * vertx + x].Normz = 0.0;
		}
	}

	//calcula los uv's
	for (int z = 0; z < vertz; z++)
	{
		for (int x = 0; x < vertx; x++)
		{
			verticesxyzSD[z * vertx + x].u = (float)x / (vertx - 1);
			verticesxyzSD[z * vertx + x].v = (float)z / (vertz - 1);
		}
	}

	glm::vec3 aux;
	//crea las normales
	for (int z = 0; z < (vertz - 1); z++)
	{
		for (int x = 0; x < (vertx - 1); x++)
		{
			aux = genNormal(&verticesxyzSD[z * vertx + x].Posx, &verticesxyzSD[(z + 1) * vertx + (x + 1)].Posx,
				&verticesxyzSD[z * vertx + (x + 1)].Posx);

			sumaNormal(&verticesxyzSD[z * vertx + x].Normx, &aux.x);
			sumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Normx, &aux.x);
			sumaNormal(&verticesxyzSD[z * vertx + (x + 1)].Normx, &aux.x);

			aux = genNormal(&verticesxyzSD[z * vertx + x].Posx, &verticesxyzSD[(z + 1) * vertx + x].Posx,
				&verticesxyzSD[(z + 1) * vertx + (x + 1)].Posx);

			sumaNormal(&verticesxyzSD[z * vertx + x].Normx, &aux.x);
			sumaNormal(&verticesxyzSD[(z + 1) * vertx + x].Normx, &aux.x);
			sumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Normx, &aux.x);
		}
	}

	//Normaliza las normales
	for (int z = 0; z < vertz; z++)
	{
		for (int x = 0; x < vertx; x++)
		{
			normaliza(&verticesxyzSD[z * vertx + x].Normx);
		}
	}

	//ahora la parte mas importante de crear vertices es el algoritmo para unirlos, en este caso sustituiremos
	//a un algoritmo con un un grupo de indices
	int indice = 0;
	for (int i = 0; i < vertz - 1; i++)
	{
		for (int j = 0; j < vertx - 1; j++)
		{
			indices[indice++] = i * vertz + j;
			indices[indice++] = (i + 1) * vertz + j + 1;
			indices[indice++] = i * vertz + j + 1;

			indices[indice++] = i * vertz + j;
			indices[indice++] = (i + 1) * vertz + j;
			indices[indice++] = (i + 1) * vertz + j + 1;
		}
	}

	//generamos un objeto para poder transportar los punteros

	Maya salida;

	salida.maya = verticesxyzSD;
	salida.indices = indices;

	return salida;
}

UTILITIES_OGL::Maya UTILITIES_OGL::Plano(int vertx, int vertz, float anchof, float profz, unsigned char* altura, int nrComponents, float tile) {
	//Cargamos la estructura con los espacios de memoria necesarios
	Vertices* verticesxyzSD = new Vertices[vertx * vertz * 3];
	unsigned int* indices = new unsigned int[vertx * vertz * 6];

	//es la separacion entre vertices, se le resta 1 para que el lado correcto
	//imagine que el ancho es de 10 y tiene 10 vertices, entonces le daria un deltax
	//de 1, si los vertices van de 0 a 9 entonces la posicion del ultimo vertice
	//seria 9, si le divide entre vertx -1 le dara 1.1111, y el ultimo vertice sera 10
	float deltax = anchof / (vertx - 1);
	float deltaz = profz / (vertz - 1);

	//crea los vertices
	for (int z = 0; z < vertz; z++)
	{
		for (int x = 0; x < vertx; x++)
		{
			verticesxyzSD[z * vertx + x].Posx = (float)x * deltax - anchof / 2.0;
			verticesxyzSD[z * vertx + x].Posy = (float)altura[z * vertx * nrComponents + x * nrComponents] / 10.0; // nrComponents -> 4
			verticesxyzSD[z * vertx + x].Posz = (float)z * deltaz - profz / 2.0;

			//carga las normales con cero
			verticesxyzSD[z * vertx + x].Normx = 0.0;
			verticesxyzSD[z * vertx + x].Normy = 1.0;
			verticesxyzSD[z * vertx + x].Normz = 0.0;
		}
	}

	//calcula los uv's
	for (int z = 0; z < vertz; z++)
	{
		for (int x = 0; x < vertx; x++)
		{
			verticesxyzSD[z * vertx + x].u = (float)(x * tile) / (vertx - 1);
			verticesxyzSD[z * vertx + x].v = (float)(z * tile) / (vertz - 1);
		}
	}

	glm::vec3 aux;
	//crea las normales
	for (int z = 0; z < (vertz - 1); z++)
	{
		for (int x = 0; x < (vertx - 1); x++)
		{
			aux = genNormal(&verticesxyzSD[z * vertx + x].Posx, &verticesxyzSD[z * vertx + (x + 1)].Posx,
				&verticesxyzSD[(z + 1) * vertx + (x + 1)].Posx);

			sumaNormal(&verticesxyzSD[z * vertx + x].Normx, &aux.x);
			sumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Normx, &aux.x);
			sumaNormal(&verticesxyzSD[z * vertx + (x + 1)].Normx, &aux.x);

			aux = genNormal(&verticesxyzSD[z * vertx + x].Posx, &verticesxyzSD[(z + 1) * vertx + x + 1].Posx,
				&verticesxyzSD[(z + 1) * vertx + x].Posx);

			sumaNormal(&verticesxyzSD[z * vertx + x].Normx, &aux.x);
			sumaNormal(&verticesxyzSD[(z + 1) * vertx + x].Normx, &aux.x);
			sumaNormal(&verticesxyzSD[(z + 1) * vertx + (x + 1)].Normx, &aux.x);
		}
	}

	//Normaliza las normales
	for (int z = 0; z < vertz; z++)
	{
		for (int x = 0; x < vertx; x++)
		{
			normaliza(&verticesxyzSD[z * vertx + x].Normx);

		}
	}


	//ahora la parte mas importante de crear vertices es el algoritmo para unirlos, en este caso sustituiremos
	//a un algoritmo con un un grupo de indices
	int indice = 0;
	for (int i = 0; i < vertz - 1; i++)
	{
		for (int j = 0; j < vertx - 1; j++)
		{
			indices[indice++] = i * vertz + j;
			indices[indice++] = (i + 1) * vertz + j + 1;
			indices[indice++] = i * vertz + j + 1;


			indices[indice++] = (i + 1) * vertz + j;
			indices[indice++] = (i + 1) * vertz + j + 1; indices[indice++] = i * vertz + j;
		}
	}

	//generamos un objeto para poder transportar los punteros

	Maya salida;

	salida.maya = verticesxyzSD;
	salida.indices = indices;

	return salida;
}

unsigned char* loadFile(char const* filename, int* x, int* y, int* comp, int req_comp, bool rotateX, bool rotateY) {
	unsigned char* data = NULL, * tmp = NULL;
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(filename, 0);
	FIBITMAP* imagen = FreeImage_Load(formato, filename);
	if (imagen != NULL) {
		if (rotateY) FreeImage_FlipVertical(imagen);
		if (rotateX) FreeImage_FlipHorizontal(imagen);
		*x = FreeImage_GetWidth(imagen);
		*y = FreeImage_GetHeight(imagen);
		// calculate the number of bytes per pixel
		*comp = FreeImage_GetLine(imagen) / FreeImage_GetWidth(imagen);
		// calculate the number of samples per pixel
		unsigned samples = *comp / sizeof(FreeImage_GetImageType(imagen));
		tmp = new unsigned char[(*x) * (*y) * (*comp)];
		memcpy(tmp, (unsigned char*)FreeImage_GetBits(imagen), (*x) * (*y) * (*comp));
		for (int j = 0; j < (*x) * (*y) && (*comp) >= 3; j++) {
			unsigned char c = tmp[j * (*comp) + 0];
			tmp[j * (*comp) + 0] = tmp[j * (*comp) + 2];
			//tmp[j * (*comp) + 1] = tmp[j * (*comp) + 1];
			tmp[j * (*comp) + 2] = c;
			//tmp[j * (*comp) + 3] = tmp[j * (*comp) + 3];
		}
		FreeImage_Unload(imagen);
	}
	else {
		if (rotateY) stbi_set_flip_vertically_on_load(false);
		data = stbi_load(filename, x, y, comp, 0);
		if (data) {
			tmp = new unsigned char[(*x) * (*y) * (*comp)];
			memcpy(tmp, data, (*x) * (*y) * (*comp));
			stbi_image_free(data);
		}
		stbi_set_flip_vertically_on_load(false);
	}
	return tmp;
}

unsigned int TextureFromFile(HWND hWnd, const char* path, const std::string& directory, bool rotateX, bool rotateY, bool alpha, struct UTILITIES_OGL::ImageDetails* img) {
	std::string filename = std::string(path);
	if (!directory.empty())
		filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = loadFile(filename.c_str(), &width, &height, &nrComponents, 0, rotateX, rotateY);
	GLenum format;
	if (data) {
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3) {
			format = GL_RGB;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}
		else if (nrComponents == 4) {
			format = GL_RGBA;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
		if (alpha) {
			format = GL_RGBA;
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		//        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);   //Requires GL 1.4. Removed from GL 3.1 and above.
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		delete[]data;
	}
	else {
		std::wstring err(L"Texture failed to load at path: ");
		err.append(s2ws(path));
		MessageBox(hWnd, err.c_str(), L"ERROR LOAD OBJ", 0);
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	if (img != NULL) {
		img->format = format;
		img->height = height;
		img->nrComponents = nrComponents;
		img->width = width;
	}
	return textureID;
}
