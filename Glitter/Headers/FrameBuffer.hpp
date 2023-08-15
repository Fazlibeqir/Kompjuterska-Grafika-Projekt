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
    FrameBuffer(const std::string& skyVertPath, const std::string& skyFragPath,const std::string& heightVertPath, const std::string& heightFragPath);
    void frameBufferInitSkyBox();
    void frameBufferRenderSkyBox();
    void frameBufferInitTerrian();
    void frameBufferRenderTerrian();
    void frameBufferInitTextures();
    float getHeightFromHeightMap(float x, float z);
    static GLuint loadTexture(GLchar* path);
    static GLuint loadCubemap(std::vector<const GLchar*> faces);
    GLuint texture{};
    GLuint terrainTexture{};
    GLuint VBO{}, VAO{}, EBO{};
    GLuint skyboxVAO{}, skyboxVBO{};
    GLuint terrainVAO, terrainVBO, terrainIBO;
    GLuint cubemapTexture{};
    Shader skyboxShader;
    Shader mapShader;
    Skybox entity;
    glm::mat4 view{};
    glm::mat4 model{};
    glm::mat4 projection{};
    glm::mat4 skyboxModel{};
    glm::mat4 skyboxView{};
    std::vector<float> vertices;
    int numStrips;
     int numTrisPerStrip;
    int width, height, nrChannels;
    unsigned bytePerPixel;
    unsigned char *data;

};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_FRAMEBUFFER_HPP
