////
//// Created by Beqir on 20.7.2023.
////
//
//#include "Skybox.hpp"
//Skybox::Skybox(const std::string &skyboxShaderVertexPath,
//               const std::string &skyboxShaderFragmentPath)
//               : skyBoxShader(skyboxShaderVertexPath.c_str(), skyboxShaderFragmentPath.c_str()){
//
//}
//const std::vector<GLfloat> &Skybox::getSkyboxVertices() const {
//    return skyboxVertices;
//}
//
//
//unsigned int Skybox:: generateBuffers(unsigned int cubeMapTexture,unsigned int &skyboxVAO,unsigned int &skyboxVBO, unsigned int& skyboxEBO){
//    skyBoxShader.Use();
//    skyBoxShader.setInt("skybox",0);
//    glGenVertexArrays(1, &skyboxVAO);
//    glGenBuffers(1, &skyboxVBO);
//    glGenBuffers(1, &skyboxEBO);
//    glBindVertexArray(skyboxVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//    string right = string(SKY_DIR) + "\\skybox2\\clouds1_east.bmp";
//    string left = string(SKY_DIR) + "\\skybox2\\clouds1_west.bmp";
//    string top = string(SKY_DIR) + "\\skybox2\\clouds1_up.bmp";
//    string bottom = string(SKY_DIR) + "\\skybox2\\clouds1_down.bmp";
//    string back = string(SKY_DIR) + "\\skybox2\\clouds1_south.bmp";
//    string front = string(SKY_DIR) + "\\skybox2\\clouds1_north.bmp";
//    std::string facesCubemap[6] =
//            {
//                    right,
//                    left,
//                    top,
//                    bottom,
//                    front,
//                    back
//            };
//
//    glGenTextures(1, &cubeMapTexture);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    // These are very important to prevent seams
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//    // This might help with seams on some systems
//    //glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
//    for (unsigned int i = 0; i < 6; i++)
//    {
//        int width, height, nrChannels;
//        unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
//        if (data)
//        {
//            glTexImage2D
//                    (
//                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
//                            0,
//                            GL_RGB,
//                            width,
//                            height,
//                            0,
//                            GL_RGB,
//                            GL_UNSIGNED_BYTE,
//                            data
//                    );
//            stbi_image_free(data);
//        }
//        else
//        {
//            std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
//            stbi_image_free(data);
//        }
//    }
//}
