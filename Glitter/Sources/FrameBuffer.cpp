//
// Created by Beqir on 27.7.2023.
//

#include "FrameBuffer.hpp"
FrameBuffer::FrameBuffer(const std::string &skyVertPath, const std::string &skyFragPath,const std::string& heightVertPath, const std::string& heightFragPath)
            :skyboxShader(skyVertPath.c_str(),skyFragPath.c_str()),mapShader(heightVertPath.c_str(),heightFragPath.c_str())
{

}
void FrameBuffer::frameBufferInitSkyBox() {
    const std::vector<GLfloat>& vertices=entity.getVertices();
    const std::vector<GLuint>& indices=entity.getIndices();
    const std::vector<GLfloat>& skyboxVertices=entity.getSkyboxVertices();
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // !!! Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Set vertices position pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(0);

    // Set vertices color pointer
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glEnable( GL_LINE_SMOOTH );
    glBindVertexArray(0);

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size()*sizeof(GLfloat), skyboxVertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)nullptr);
    glBindVertexArray(0);
}
void FrameBuffer::frameBufferInitTerrian() {
    const std::string mapPath= std::string(SKY_DIR)+"\\terrian\\WorldGrayscale_Maproom.jpg";

    data = stbi_load(mapPath.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    float yScale = 64.0f / 256.0f, yShift = 16.0f;
    int rez = 1;
    bytePerPixel = nrChannels;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
            unsigned char y = pixelOffset[0];

            // vertex
            vertices.push_back( -height/2.0f + height*i/(float)height );   // vx
            vertices.push_back( (int) y * yScale - yShift);   // vy
            vertices.push_back( -width/2.0f + width*j/(float)width );   // vz
        }
    }
    stbi_image_free(data);

    std::vector<unsigned> indices;
    for(unsigned i = 0; i < height-1; i += rez)
    {
        for(unsigned j = 0; j < width; j += rez)
        {
            for(unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + width * (i + k*rez));
            }
        }
    }

    numStrips = (height-1)/rez;
    numTrisPerStrip = (width/rez)*2-2;


    // first, configure the cube's VAO (and terrainVBO + terrainIBO)

    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

    glGenTextures(1, &terrainTexture);
    glBindTexture(GL_TEXTURE_2D, terrainTexture);
}
float FrameBuffer::getHeightFromHeightMap(float  x, float z) {
   float texCoordX =(x+ width/2.0f)/width;
   float texCoordZ=(z+ height/2.0f)/height;

   texCoordX=glm::clamp(texCoordX,0.0f,1.0f);
   texCoordZ=glm::clamp(texCoordZ,0.0f,1.0f);

   int pixelX=static_cast<int>(texCoordX* (width - 1));
   int pixelZ=static_cast<int>(texCoordZ*(height - 1));

    if (pixelX < 0 || pixelX >= width-1 || pixelZ < 0 || pixelZ >= height-1) {
        // Handle out-of-bounds access gracefully
        return 0.0f; // or another appropriate value
    }
  int vertexOffset=(pixelZ*width+pixelX)*3;

    float y=vertices[vertexOffset+1];

   float yScale=64.0f/256.0f;
   float yShift=16.0f;

   return y*yScale-yShift;
}

void FrameBuffer::frameBufferInitTextures(){
    std::string right = std::string(SKY_DIR) + "\\skybox\\right.png";
    std::string left = std::string(SKY_DIR) + "\\skybox\\left.png";
    std::string top = std::string(SKY_DIR) + "\\skybox\\top.png";
    std::string bottom = std::string(SKY_DIR) + "\\skybox\\bottom.png";
    std::string back = std::string(SKY_DIR) + "\\skybox\\back.png";
    std::string front = std::string(SKY_DIR) + "\\skybox\\front.png";

    std::vector<const GLchar*> faces;
    faces.push_back(right.c_str());
    faces.push_back(left.c_str());
    faces.push_back(top.c_str());
    faces.push_back(bottom.c_str());
    faces.push_back(front.c_str());
    faces.push_back(back.c_str());

    cubemapTexture = loadCubemap(faces);

    std::string texture0=std::string(SKY_DIR)+"\\textures\\text0.jpg";
    texture = loadTexture(const_cast<GLchar *>(texture0.c_str()));
//    std::string texture1=std::string(SKY_DIR)+"\\textures\\iclandColor.png";
//    terrainTexture = loadTexture(const_cast<GLchar *>(texture1.c_str()));
}
void FrameBuffer::frameBufferRenderSkyBox() {
    glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
    skyboxShader.use();
    view = glm::mat4(glm::mat3(camera.GetViewMatrix()));    // Remove any translation component of the view matrix
    skyboxShader.setMat4("view",view);
    skyboxShader.setMat4("projection",projection);
    // skybox cube


    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    skyboxShader.setInt("skybox",0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // Set depth function back to default
}
void FrameBuffer::frameBufferRenderTerrian() {
    mapShader.use();

    // view/projection transformations
     projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);
     view = camera.GetViewMatrix();
    mapShader.setMat4("projection", projection);
    mapShader.setMat4("view", view);

    // world transformation
    glm::mat4 modelTerrian = glm::mat4(1.0f);
    modelTerrian = glm::translate(modelTerrian, glm::vec3(0.0f, 0.0f, 0.0f));
    mapShader.setMat4("model", modelTerrian);
    mapShader.setFloat("width", static_cast<float>(width));
    mapShader.setFloat("height", static_cast<float>(height));


    mapShader.setInt("terrianTexture",0);
    glBindTexture(GL_TEXTURE_2D, terrainTexture);

    // render the cube
    glBindVertexArray(terrainVAO);
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(unsigned strip = 0; strip < numStrips; strip++)
    {
        glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
                       numTrisPerStrip+2,   // number of indices to render
                       GL_UNSIGNED_INT,     // index data type
                       (void*)(sizeof(unsigned) * (numTrisPerStrip+2) * strip)); // offset to starting index
    }

}

GLuint  FrameBuffer::loadCubemap(std::vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, channels;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (GLuint i = 0; i < faces.size(); i++)
    {
        image = stbi_load(faces[i], &width, &height, &channels, 0);
        GLint  format;
        switch (channels)
        {
            case 1: format = GL_ALPHA;     break;
            case 2: format = GL_LUMINANCE; break;
            case 3: format = GL_RGB;       break;
            case 4: format = GL_RGBA;      break;
            default: format = GL_RGB;      break;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}
GLuint  FrameBuffer::loadTexture(GLchar* path)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height, channels;
    unsigned char* image = stbi_load(path, &width, &height, &channels, 0);
    GLint format;
    switch (channels)
    {
        case 1: format = GL_ALPHA;     break;
        case 2: format = GL_LUMINANCE; break;
        case 3: format = GL_RGB;       break;
        case 4: format = GL_RGBA;      break;
        default: format = GL_RGB;      break;
    }
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
    return textureID;
}