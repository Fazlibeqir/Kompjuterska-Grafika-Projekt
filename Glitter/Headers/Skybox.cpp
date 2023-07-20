//
// Created by Beqir on 20.7.2023.
//

#include "Skybox.hpp"

Skybox::Skybox() {
    vertices = {
            //    Position + Normal + TextOrd
            114.5f,  -1.7f, 111.5f,   0.0f, 0.0f, 0.0f,
            114.5f,  -1.7f, -111.5f,  0.0f, 1.0f, 0.0f,
            -112.0f, -1.7f, 111.5f,   0.0f, 0.0f, 1.0f,
            -112.0f, -1.7f, -111.5f,  0.0f, 1.0f, 1.0f,

    };
     indices = {0, 1, 2, 3,};
     skyboxVertices = {            // Positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };


}

const std::vector<GLfloat> &Skybox::getVertices() const {
    return vertices;
}

const std::vector<GLuint> &Skybox::getIndices() const {
    return indices;
}

const std::vector<GLfloat> &Skybox::getSkyboxVertices() const {
    return skyboxVertices;
}
