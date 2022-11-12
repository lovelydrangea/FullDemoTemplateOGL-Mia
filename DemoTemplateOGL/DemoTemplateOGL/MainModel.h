#ifndef MAINMODEL_H
#define MAINMODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Utilities.h"
#include "Model.h"
#include "camera.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class MainModel : public Model, public Camera {
public:
    MainModel();
    MainModel(HWND hWnd, string const& path, bool rotationX = false, bool rotationY = true, bool gamma = false);
    MainModel(HWND hWnd, string const& path, glm::vec3 actualPosition, bool rotationX = false, bool rotationY = true, bool gamma = false);
    glm::mat4 makeTransNextPosition();
    glm::mat4 makeTransScaleNextPosition(const glm::mat4& prevTransformations);
    glm::mat4 CamaraUpdate();
    bool colisionaCon(Model* objeto);
    std::pair<Node*, Node*> nodoColisionCon(Model* objeto);
//    void CamaraGiraY(float grados);
//    glm::mat4 GetViewMatrix();
//    glm::mat4 CamaraUpdate();
//    void CamaraAvanza(float vel);
};

#endif