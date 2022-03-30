#include "Model.h"

Model::Model() {
    this->hWnd = NULL;
    this->cameraDetails = NULL;
    this->gammaCorrection = false;
}
Model::Model(HWND hWnd, string const& path, Camera* camera, bool rotationX, bool rotationY, bool gamma){
    this->cameraDetails = camera;
    gammaCorrection = gamma;
    loadModel(hWnd, path, rotationX, rotationY);
    defaultShader = false;
    buildKDtree();
}

Model::~Model() {
    if (gpuDemo != NULL) {
        delete gpuDemo;
        gpuDemo = NULL;
    }
    for (int i = 0; i < textures_loaded.size(); i++) {
        glDeleteTextures(1, &(textures_loaded[i].id));
    }
}
// draws the model, and thus all its meshes
void Model::prepShader(Shader& gpuDemo) {
    glm::vec3 lightColor;
    lightColor.x = 3;//sin(7200 * 2.0f);
    lightColor.y = 3;//sin(7200 * 0.7f);
    lightColor.z = 3;//sin(7200 * 1.3f);
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
    gpuDemo.setVec3("light.ambient", ambientColor);
    gpuDemo.setVec3("light.diffuse", diffuseColor);
    gpuDemo.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    //        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 lightPos(1.2f, 100.0f, 2.0f);
    gpuDemo.setVec3("light.position", lightPos);
    gpuDemo.setVec3("viewPos", cameraDetails->getPosition());

    // view/projection transformations
    gpuDemo.setMat4("projection", cameraDetails->getProjection());
    gpuDemo.setMat4("view", cameraDetails->getView());

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    if (hasTranslate)
        model = glm::translate(model, translate); // translate it down so it's at the center of the scene
//			model = glm::translate(model, glm::vec3(cameraDetails.Position->x, cameraDetails.Position->y - 5, cameraDetails.Position->z)); // translate it down so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(0.0025f, 0.0025f, 0.0025f));	// it's a bit too big for our scene, so scale it down
    if (hasScale)
        model = glm::scale(model, scale);	// it's a bit too big for our scene, so scale it down
    gpuDemo.setMat4("model", model);
}
void Model::Draw() {
    if (gpuDemo == NULL) {
        gpuDemo = new Shader("shaders/models/1.model_material_loading.vs", "shaders/models/1.model_material_loading.fs");
        defaultShader = true;
    }
    if (defaultShader) {
        gpuDemo->use();
        prepShader(*gpuDemo);
        Draw(*gpuDemo);
        gpuDemo->desuse();
    }
    else gpuDemo->desuse();
}
void Model::Draw(Shader& shader) {
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}
glm::mat4 Model::makeTransScale(const glm::mat4& prevTransformations) const {
    if (hasScale)
        return prevTransformations * glm::scale(makeTrans(), scale);
    else
        return prevTransformations * makeTrans();
}
glm::mat4 Model::makeTrans() const {
    return  glm::translate(glm::mat4(1), translate);//glm::mat4(1) *glm::mat4(1)* glm::mat4(1);
}
HWND Model::getHWND() { return this->hWnd; }
void Model::setHWND(HWND hWnd) { this->hWnd = hWnd; }
bool Model::getDefaultShader() { return this->defaultShader; }
void Model::setDefaultShader(bool defaultShader) { this->defaultShader = defaultShader; }

void Model::setTranslate(glm::vec3* translate) {
    if (translate == NULL) {
        this->translate = glm::vec3(0);
        this->hasTranslate = false;
    }
    else {
        this->translate = *translate;
        this->hasTranslate = true;
    }
}
void Model::setScale(glm::vec3* scale) {
    if (scale == NULL) {
        this->scale = glm::vec3(0);
        this->hasScale = false;
    }
    else {
        this->scale = *scale;
        this->hasScale = true;
    }
}

void Model::buildKDtree() {
    std::list<Node::vecType> point_list;
    for (int i = 0; i < meshes.size(); i++)
        for (int j = 0; j < meshes[i].vertices.size(); j++)
            point_list.push_back(Node::vecType(meshes[i].vertices[j].Position, 1));
    kdTree.makeTree(point_list);
}

vector<Material> Model::loadMaterial(aiMaterial* mat) {
    Material material;
    ZeroMemory(&material, sizeof(material));
    aiColor3D color(0.f, 0.f, 0.f);
    float shininess;
    bool matFound = false;
    if (!mat->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
        material.Diffuse = glm::vec3(color.r, color.b, color.g);
        matFound = true;
        material.hasDiffuse = true;
    }
    else material.hasDiffuse = false;

    if (!mat->Get(AI_MATKEY_COLOR_AMBIENT, color)) {
        material.Ambient = glm::vec3(color.r, color.b, color.g);
        matFound = true;
        material.hasAmbient = true;
    }
    else material.hasAmbient = false;

    if (!mat->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
        material.Specular = glm::vec3(color.r, color.b, color.g);
        matFound = true;
        material.hasSpecular = true;
    }
    else material.hasSpecular = false;

    if (!mat->Get(AI_MATKEY_SHININESS, shininess)) {
        material.Shininess = shininess;
        matFound = true;
        material.hasShininess = true;
    }
    else material.hasShininess = false;
    vector<Material> m;
    if (matFound) {
        material_loaded.push_back(material);
        m.push_back(material);
    }
    return m;
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void Model::loadModel(HWND hWnd, string const& path, bool rotationX, bool rotationY)
{
    this->hWnd = hWnd;
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        wstring err(L"ERROR::ASSIMP:: ");
        err.append(s2ws(importer.GetErrorString()));
        MessageBox(hWnd, err.c_str(), L"ERROR LOAD OBJ", 0);
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));
    name.assign(path.substr(path.find_last_of('/') + 1));
    name.erase(name.find_last_of('.'));
    // process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene, rotationX, rotationY);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode* node, const aiScene* scene, bool rotationX, bool rotationY)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, rotationX, rotationY));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, rotationX, rotationY);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, bool rotationX, bool rotationY)
{
    // data to fill
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    vector<Material> materials;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<Material> mat = loadMaterial(material);
    materials.insert(materials.end(), mat.begin(), mat.end());
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", rotationX, rotationY);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", rotationX, rotationY);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", rotationX, rotationY);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", rotationX, rotationY);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    return Mesh(vertices, indices, textures, materials);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, bool rotationX, bool rotationY)
{
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(hWnd, str.C_Str(), this->directory, rotationX, rotationY);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}
