//
// Created by Beqir on 27.7.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_FRAMEBUFFER_HPP
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_FRAMEBUFFER_HPP

#include "init.h"
#include "shader.hpp"
#include "Skybox.hpp"

class FrameBuffer {
public:
    FrameBuffer(const std::string& skyVertPath, const std::string& skyFragPath);
    void frameBufferInitSkyBox();
    void frameBufferRenderSkyBox();
    void frameBufferInitTextures();
    static GLuint loadTexture(GLchar* path);
    static GLuint loadCubemap(std::vector<const GLchar*> faces);
    GLuint texture{};
    GLuint VBO{}, VAO{}, EBO{};
    GLuint skyboxVAO{}, skyboxVBO{};
    GLuint cubemapTexture{};
    Shader skyboxShader;
    Skybox entity;
    glm::mat4 view{};
    glm::mat4 model{};
    glm::mat4 projection{};

};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_FRAMEBUFFER_HPP
