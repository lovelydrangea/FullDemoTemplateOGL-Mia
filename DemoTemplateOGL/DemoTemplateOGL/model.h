#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Utilities.h"
#include "material.h"
#include "mesh.h"
#include "shader.h"
#include "CollitionDetection.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "camera.h"
using namespace std;

class Model {
private:
    bool hasTranslate;
    glm::vec3 translate;
    bool hasScale;
    glm::vec3 scale;
    float rotacionAngle;
    glm::vec3 rotation;
    HWND hWnd;
    bool defaultShader;
public:
    // model data 
    Camera* cameraDetails = NULL;
    Model* AABB = NULL; // Modelo que alberga un cubo para colisiones

    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Material> material_loaded;	// stores all the materials.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;
    Shader* gpuDemo;
    string name;
    KDTree kdTree;
    friend class MainModel;
    // constructor, expects a filepath to a 3D model.
    Model();
    Model(HWND hWnd, string const& path, Camera* camera, bool rotationX = false, bool rotationY = true, bool gamma = false);
    Model(HWND hWnd, vector<Vertex> vertices, unsigned int numVertices, vector<unsigned int> indices, unsigned int numIndices);
    ~Model();
    // draws the model, and thus all its meshes
    void prepShader(Shader& gpuDemo);
    void Draw();
    void Draw(Shader& shader);
    glm::mat4 makeTransScale(const glm::mat4& prevTransformations) const;
    glm::mat4 makeTrans() const;
    HWND getHWND();
    void setHWND(HWND hWnd);
    bool getDefaultShader();
    void setDefaultShader(bool defaultShader);

    void setTranslate(glm::vec3* translate);
    void setScale(glm::vec3* scale);
    void setRotation(float rotationAngle, glm::vec3* rotationVector);
    glm::vec3* getTranslate();
    glm::vec3* getScale();
    float getRotationAngle();
    glm::vec3* getRotationVector();

    void buildKDtree();
    bool colisionaCon(Model* objeto);
    std::pair<Node*, Node*> nodoColisionCon(Model* objeto);
private:
    vector<Vertex> init_cube(float x, float y, float z, float width, float height, float depth);
    vector<unsigned int> getCubeIndex();
    vector<Material> loadMaterial(aiMaterial* mat);
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(HWND hWnd, string const& path, bool rotationX = false, bool rotationY = true);
    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene, bool rotationX = false, bool rotationY = true);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, bool rotationX = false, bool rotationY = true);
    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, bool rotationX = false, bool rotationY = true);
};

#endif