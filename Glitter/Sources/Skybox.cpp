//
// Created by Beqir on 20.7.2023.
//

#include "Skybox.hpp"

Skybox::Skybox(const std::string &skyboxShaderVertexPath,
               const std::string &skyboxShaderFragmentPath)
               : skyBoxShader(skyboxShaderVertexPath.c_str(), skyboxShaderFragmentPath.c_str()){
     skyboxVertices = {
            // positions
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,

            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };

}

//Skybox::Skybox() {
//    vertices = {
//            //    Position + Normal + TextOrd
//            114.5f,  -1.7f, 111.5f,   0.0f, 0.0f, 0.0f,
//            114.5f,  -1.7f, -111.5f,  0.0f, 1.0f, 0.0f,
//            -112.0f, -1.7f, 111.5f,   0.0f, 0.0f, 1.0f,
//            -112.0f, -1.7f, -111.5f,  0.0f, 1.0f, 1.0f,
//
//    };
//     indices = {0, 1, 2, 3,};
//     skyboxVertices = {            // Positions
//            -1.0f,  1.0f, -1.0f,
//            -1.0f, -1.0f, -1.0f,
//            1.0f, -1.0f, -1.0f,
//            1.0f, -1.0f, -1.0f,
//            1.0f,  1.0f, -1.0f,
//            -1.0f,  1.0f, -1.0f,
//
//            -1.0f, -1.0f,  1.0f,
//            -1.0f, -1.0f, -1.0f,
//            -1.0f,  1.0f, -1.0f,
//            -1.0f,  1.0f, -1.0f,
//            -1.0f,  1.0f,  1.0f,
//            -1.0f, -1.0f,  1.0f,
//
//            1.0f, -1.0f, -1.0f,
//            1.0f, -1.0f,  1.0f,
//            1.0f,  1.0f,  1.0f,
//            1.0f,  1.0f,  1.0f,
//            1.0f,  1.0f, -1.0f,
//            1.0f, -1.0f, -1.0f,
//
//            -1.0f, -1.0f,  1.0f,
//            -1.0f,  1.0f,  1.0f,
//            1.0f,  1.0f,  1.0f,
//            1.0f,  1.0f,  1.0f,
//            1.0f, -1.0f,  1.0f,
//            -1.0f, -1.0f,  1.0f,
//
//            -1.0f,  1.0f, -1.0f,
//            1.0f,  1.0f, -1.0f,
//            1.0f,  1.0f,  1.0f,
//            1.0f,  1.0f,  1.0f,
//            -1.0f,  1.0f,  1.0f,
//            -1.0f,  1.0f, -1.0f,
//
//            -1.0f, -1.0f, -1.0f,
//            -1.0f, -1.0f,  1.0f,
//            1.0f, -1.0f, -1.0f,
//            1.0f, -1.0f, -1.0f,
//            -1.0f, -1.0f,  1.0f,
//            1.0f, -1.0f,  1.0f
//    };
//}

//const std::vector<GLfloat> &Skybox::getVertices() const {
//    return vertices;
//}
//
//const std::vector<GLuint> &Skybox::getIndices() const {
//    return indices;
//}
//
const std::vector<GLfloat> &Skybox::getSkyboxVertices() const {
    return skyboxVertices;
}

unsigned int Skybox:: loadCubeMap() {
    string right = string(SKY_DIR) + "\\skybox\\right.png";
    string left = string(SKY_DIR) + "\\skybox\\left.png";
    string top = string(SKY_DIR) + "\\skybox\\top.png";
    string bottom = string(SKY_DIR) + "\\skybox\\bottom.png";
    string back = string(SKY_DIR) + "\\skybox\\back.png";
    string front = string(SKY_DIR) + "\\skybox\\front.png";
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, channels;
    unsigned char *data;
    std::vector<std::string> txt_faces;
    txt_faces.push_back(right.c_str());
    txt_faces.push_back(left.c_str());
    txt_faces.push_back(top.c_str());
    txt_faces.push_back(bottom.c_str());
    txt_faces.push_back(front.c_str());
    txt_faces.push_back(back.c_str());
    for (unsigned int i = 0; i < 6; i++) {
        data = stbi_load(txt_faces[i].c_str(), &width, &height, &channels, 0);
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Skybox:: generateBuffers(unsigned int &skyboxVAO,unsigned int &skyboxVBO){
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
}
