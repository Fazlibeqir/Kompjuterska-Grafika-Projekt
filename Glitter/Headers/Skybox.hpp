//
// Created by Beqir on 20.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_SKYBOX_HPP
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_SKYBOX_HPP
#pragma once

#include "glad/glad.h"
#include <vector>


class Skybox {
public:
    Skybox();

    const std::vector<GLfloat> &getVertices() const;

    const std::vector<GLuint> &getIndices() const;

    const std::vector<GLfloat> &getSkyboxVertices() const;


private:
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> skyboxVertices;

};



#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_SKYBOX_HPP
