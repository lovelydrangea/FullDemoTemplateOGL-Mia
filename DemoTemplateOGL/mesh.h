#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Utilities.h"
#include "material.h"
#include "shader.h"

#include <string>
#include <vector>
#include <list>
#include "KDTree.h"
using namespace std;

class Mesh {
public:
    //En honor a nuestros ancestros llamaremos "Maya" a la malla
    //e suna estructura que contiene a los indices y vertices de la figura
    //el nombre numerico de la textura en cuestion, por lo pronto una

    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    vector<Material>     materials;
    unsigned int VAO;

    // constructor
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, vector<Material> materials) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->materials = materials;
        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    // render the mesh
    void Draw(Shader& shader) {
        // bind appropriate textures
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        //        glEnable(GL_TEXTURE_2D);
        for (unsigned int i = 0; i < textures.size() || i < materials.size(); i++) {
            if (i < textures.size()) {
                glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)
                string number;
                string name = textures[i].type;
                if (name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if (name == "texture_specular")
                    number = std::to_string(specularNr++); // transfer unsigned int to stream
                else if (name == "texture_normal")
                    number = std::to_string(normalNr++); // transfer unsigned int to stream
                else if (name == "texture_height")
                    number = std::to_string(heightNr++); // transfer unsigned int to stream
                shader.setInt("textureSample", 1);
                // now set the sampler to the correct texture unit
                glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
                // and finally bind the texture
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            } else shader.setInt("textureSample", 0);
            if (i < materials.size()) {
                Material m_terial = materials[i];
                if (m_terial.hasAmbient && !(zeroVec3(m_terial.Ambient)))
                    shader.setVec3("material.ambient", m_terial.Ambient);
                else shader.setVec3("material.ambient", glm::vec3(1.0f));
                if (m_terial.hasDiffuse && !(zeroVec3(m_terial.Diffuse)))
                    shader.setVec3("material.diffuse", m_terial.Diffuse);
                else shader.setVec3("material.diffuse", glm::vec3(1.0f));
                if (m_terial.hasSpecular && !(zeroVec3(m_terial.Specular)))
                    shader.setVec3("material.specular", m_terial.Specular); // specular lighting doesn't have full effect on this object's material
                else shader.setVec3("material.specular", glm::vec3(1.0f));
                shader.setFloat("material.shininess", m_terial.Shininess == 0 ? 1.0f : m_terial.Shininess);
            }
            else { // Default material for texture
                shader.setVec3("material.ambient", glm::vec3(1.0f));
                shader.setVec3("material.diffuse", glm::vec3(1.0f));
                shader.setVec3("material.specular", glm::vec3(1.0f));
                shader.setFloat("material.shininess", 1.0f);
            }
        }

        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (const void*)indices[0]);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
        //        glDisable(GL_TEXTURE_2D);
    }

private:
    // render data 
    unsigned int VBO, EBO;
    // initializes all the buffer objects/arrays
    void setupMesh() {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Position));//0
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);
    }
};
#endif