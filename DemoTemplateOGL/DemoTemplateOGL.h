#pragma once

#include "resource.h"
#include <windows.h>
#include <windowsx.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glext.h"
#include "wglext.h"
#include <iostream>

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
};

// Estructura de Texture para guardar el ID de la textura y su tipo
extern struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

// Detalles de la camara, esta estructura contiene
// Matriz actual de proyeccion
// Vista actual
// Posicion de camara
// vector front
// vector Up
// vector right
// vector WorldUp
// Valores para la camara
extern struct CamDetails {
    glm::mat4 currentProjection;
    glm::mat4 currentView;
    glm::vec3 *Position;
    glm::vec3 *Front;
    glm::vec3 *Up;
    glm::vec3 *Right;
    glm::vec3 *WorldUp;
    float *Yaw;
    float *Pitch;
    float *MovementSpeed;
    float *MouseSensitivity;
    float *Zoom;
};
extern struct CamDetails cameraDetails;
