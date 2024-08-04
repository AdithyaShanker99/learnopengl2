#include "../Headers/HeightMap.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

heightMap::heightMap(const char* filePath)
{
    int width, height, nChannels;
    unsigned char* data = stbi_load(filePath, &width, &height, &nChannels, 0);
    if (data)
    {
        this->width = width;
        this->height = height;
        this->nChannels = nChannels;
        this->data = data;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
}

std::vector<glm::vec3> heightMap::createVertices()
{
    std::vector<glm::vec3> vertices;
    float yScale = 0.1f, yShift = 0.0f;  // apply a scale+shift to the height data
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            // retrieve texel for (i,j) tex coord
            unsigned char* texel = data + (j + width * i) * nChannels;
            // raw height at coordinate
            unsigned char y = texel[0];

            vertices.push_back(glm::vec3(-height/15.0f + i, (int)y * yScale - yShift, -width/15.0f + j));
        }
    }
    return vertices;
}

std::vector<GLuint> heightMap::createIndices()
{   
    // index generation
    std::vector<GLuint> indices;
    for(unsigned int i = 0; i < height-1; i++)       // for each row a.k.a. each strip
    {
        for(unsigned int j = 0; j < width; j++)      // for each column
        {
            for(unsigned int k = 0; k < 2; k++)      // for each side of the strip
            {
                indices.push_back(GLuint(j + width * (i + k)));
            }
        }
    }
    return indices;
}

std::vector<glm::vec3> heightMap::createNormals(const std::vector<glm::vec3>& vertices)
{
    // for(int i = 0; i < vertices.size(); i++)
    // {
    //     std::cout << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << std::endl;
    // }
    std::vector<glm::vec3> normals(vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));
    
    for(unsigned int i = 0; i < height - 1; i++)
    {
        for(unsigned int j = 0; j < width - 1; j++)
        {
            unsigned int index1 = j + width * i;
            unsigned int index2 = index1 + 1;
            unsigned int index3 = index1 + width;
            unsigned int index4 = index3 + 1;

            glm::vec3 v1 = vertices[index2] - vertices[index1];
            glm::vec3 v2 = vertices[index3] - vertices[index1];
            glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

            normals[index1] += normal;
            normals[index2] += normal;
            normals[index3] += normal;
            normals[index4] += normal;
        }
    }

    for (unsigned int i = 0; i < normals.size(); i++)
    {
        normals[i] = glm::normalize(normals[i]);
    }

    return normals;
}

std::vector<glm::vec2> heightMap::createTextureCoords(const std::vector<glm::vec3>& vertices)
{
    std::vector<glm::vec2> texCoords;
    for(const auto& vertex : vertices)
    {
        // Assuming planar mapping onto the XY plane
        glm::vec2 texCoord = glm::vec2(vertex.x, vertex.z);
        texCoords.push_back(texCoord);
    }
    return texCoords;
}


std::vector<Vertex> heightMap::createVerts(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec2>& textureCoords)
{
    std::vector<Vertex> verts;
    for(unsigned int i = 0; i < vertices.size(); i++)
    {
        Vertex vert;
        vert.position = vertices[i];
        vert.color = glm::vec3(1.0f, 1.0f, 1.0f);
        vert.normal = normals[i];
        vert.texUV = textureCoords[i]/10.0f;
        verts.push_back(vert);
    }
    return verts;
}
