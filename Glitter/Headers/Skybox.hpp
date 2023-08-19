////
//// Created by Beqir on 20.7.2023.
////

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_SKYBOX_HPP
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_SKYBOX_HPP
#pragma once

#include <vector>
#include "glad/glad.h"
#include "shader.hpp"
#include "stb_image.h"


class Skybox {
public:
//    const std::vector<GLfloat> &getVertices() const;
//    const std::vector<GLuint> &getIndices() const;
    Shader skyBoxShader;
    std::vector<GLfloat> skyboxVertices;

    Skybox(const string& terrainShaderVertexPath,
           const string& terrainShaderFragmentPath);
    const std::vector<GLfloat> &getSkyboxVertices() const;
    unsigned int loadCubeMap();
    void generateBuffers(unsigned int &skyboxVAO,unsigned int &skyboxVBO);
private:
//    std::vector<GLfloat> vertices;
//    std::vector<GLuint> indices;

};



#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_SKYBOX_HPP
