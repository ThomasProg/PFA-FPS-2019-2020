#include "model.hpp"

#include "primitives.hpp"

#include <string>
#include <iostream>
#include <fstream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

Resources::Model::Model(Model&& rhs)
{
    VAO = rhs.VAO;
    VBO = rhs.VBO;

    positions   = std::move(rhs.positions);
    normals     = std::move(rhs.normals);
    textCoords  = std::move(rhs.textCoords);
    // indices     = std::move(rhs.indices);

    // TODO : remove ? Useless ?
    rhs.VAO = 0;
    rhs.VBO = 0;
}

Resources::Model::Model(Primitives&& primitive)
{
    this->positions.reserve(primitive.indices.size());
    for (unsigned int i = 0; i < primitive.indices.size(); i++)
    {
        positions.emplace_back(primitive.vertices[primitive.indices[i]]);
    }
    this->normals    = std::move(primitive.normals);
    this->textCoords = std::move(primitive.uvs);
}

Resources::Model::Model(const char* pathToFile)
{
    loadOBJ(pathToFile);
    setupModel();
}

Resources::Model::~Model()
{
    if (VAO != 0)
        glDeleteVertexArrays(1, &VAO);

    if (VBO != 0)
        glDeleteBuffers(1, &VBO);
}

void Resources::Model::loadOBJ(const char* pathToFile)
{
    if(pathToFile == nullptr)
            return;
        
    std::ifstream file(pathToFile);
    std::vector<Core::Maths::Vec3> tempVertex;
    std::vector<Core::Maths::Vec3> tempNormal;
    std::vector<Core::Maths::Vec2> tempUV;
    std::vector<unsigned int>vertexIndices, uvIndicies, normalIndices;

    if(!file.is_open())
        std::cout << "FAIL TO READ OBJ FILE" << std::endl;

    std::string head;
    char slash;

    while(file.good())
    {
        file >> head;

        if(head == "v")
        {
            Core::Maths::Vec3 vertex;
            file >> vertex.x >> vertex.y >> vertex.z;
            tempVertex.push_back(vertex);
        }
        else if(head == "vt")
        {
            Core::Maths::Vec2 textCoord;
            file >> textCoord.x >> textCoord.y;
            tempUV.push_back(textCoord);
        }
        else if(head == "vn")
        {
            Core::Maths::Vec3 normal;
            file >> normal.x >> normal.y >> normal.z;

            tempNormal.push_back(normal);
        }
        else if(head == "f")
        {
            char line[256];

            if(!file.getline(line, 256))
                break;
            
            int i = 0;
            int nbShlash = 0;

            for(; line[i] != '\0'; i++)
            {
                if(line[i] == '/')
                    nbShlash++;
            }
            //reset cursor at start
            file.seekg(-i, file.cur);

            unsigned int vertexIndex[nbShlash / 2], uvIndex[nbShlash / 2], normalIndex[nbShlash / 2];

            file >> vertexIndex[0] >> slash >> uvIndex[0] >> slash >> normalIndex[0]
                >> vertexIndex[1] >> slash >> uvIndex[1] >> slash >> normalIndex[1]
                >> vertexIndex[2] >> slash >> uvIndex[2] >> slash >> normalIndex[2];

            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndicies.push_back(uvIndex[0]);
            uvIndicies.push_back(uvIndex[1]);
            uvIndicies.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);

            if(nbShlash == 8)
            {
                file >> vertexIndex[3] >> slash >> uvIndex[3] >> slash >> normalIndex[3];

                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[2]);
                vertexIndices.push_back(vertexIndex[3]);
                uvIndicies.push_back(uvIndex[0]);
                uvIndicies.push_back(uvIndex[2]);
                uvIndicies.push_back(uvIndex[3]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[2]);
                normalIndices.push_back(normalIndex[3]);
            }
        }
    }
    std::cout << tempVertex.size() << std::endl;
    for(unsigned int j = 0; j < vertexIndices.size(); j++)
    {
        unsigned int indexVertex = vertexIndices[j];
        // std::cout << indexVertex << std::endl;
        positions.push_back(tempVertex[indexVertex - 1]);
    }

    for(unsigned int j = 0; j < uvIndicies.size(); j++)
    {
        unsigned int indexUV = uvIndicies[j];
        textCoords.push_back(tempUV[indexUV - 1]); 
    }

    for(unsigned int j = 0; j < normalIndices.size(); j++)
    {
        unsigned int indexNormal = normalIndices[j];
        normals.push_back(tempNormal[indexNormal - 1]);
    }

    file.close();
}

void Resources::Model::setupModel()
{
    glGenBuffers(1, &VBO);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);

    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // //Texture
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, textCoords.size() * sizeof(textCoords), &textCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // //Normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals), &normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}