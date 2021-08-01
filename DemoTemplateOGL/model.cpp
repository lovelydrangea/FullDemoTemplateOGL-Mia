#include "model.h"
#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "material.h"
#include "mesh.h"
#include "shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include "Utilities.h"
#include <FreeImage.h>

using namespace std;

unsigned char* loadFile(char const* filename, int* x, int* y, int* comp, int req_comp, bool rotateX, bool rotateY) {
    unsigned char* data = NULL, *tmp = NULL;
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
    } else {
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

unsigned int TextureFromFile(HWND hWnd, const char* path, const string& directory, bool rotateX, bool rotateY, bool alpha, struct UTILITIES_OGL::ImageDetails* img) {
    string filename = string(path);
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
        delete []data;
    } else {
        wstring err(L"Texture failed to load at path: ");
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
