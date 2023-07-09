// Local Headers
//#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>

#include "shader.hpp"
#include "Model.hpp"
#include "init.h"
//#include "light.hpp"

// Standard Headers

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {

    GLFWwindow* mWindow = initializeWindow();

    string vertPath= string(SHADER_DIR)+"\\belt.vert";
    string fragmentPath= string(SHADER_DIR)+ "\\belt.frag";
    string modelPath= string(MODEL_DIR)+ "\\car\\050 Low Poly Camaro.obj";

    Shader ourShader(vertPath.c_str(),fragmentPath.c_str());
    Model car(modelPath.c_str());

    // Rendering Loop
    while (!glfwWindowShouldClose(mWindow)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(mWindow);
        // Background Fill Color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = camera.GetViewMatrix();;
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));

        ourShader.setMat4("model", model);
        car.Draw(ourShader);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}



