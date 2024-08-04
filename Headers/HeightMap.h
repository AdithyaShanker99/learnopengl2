#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include<vector>
#include<glm/glm.hpp>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Textures.h"
#include<stb_image.h>




class heightMap
{
    public:

        int width;
        int height;
        int nChannels;

        unsigned char* data;

        heightMap(const char* filePath);

        std::vector<glm::vec3> createVertices();
        std::vector<GLuint> createIndices();
        std::vector<glm::vec3> createNormals(const std::vector<glm::vec3>& vertices);
        std::vector<glm::vec2> createTextureCoords(const std::vector<glm::vec3>& vertices);
        std::vector<Vertex> createVerts(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& textureCoords);

};
#endif