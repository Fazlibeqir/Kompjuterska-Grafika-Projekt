//
// Created by Elena on 08.7.2023.
//

#include <thread>
#include "init.h"


float deltaTime = 1.0f;
float lastFrame = 0.0f;
float carSpeed = 5.0f;
float rotationAngle = 0.0f;
glm::vec3 carPosition = glm::vec3(0.0f);
glm::vec3 carFront = glm::vec3(0.0f, 0.0f, -1.0f);
float carRotationSpeed = 50.0f;
bool returnToMenuClicked= false;

Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

GLFWwindow* initializeWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* mWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Velocity Xtreme", nullptr, nullptr);

// Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        exit(1);
    }

// Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
//    glfwSetCursorPosCallback(mWindow,mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);

    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return mWindow;
}
void updateDeltaTime()
{
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}
void processInput(GLFWwindow *window, int * chosenCarIndex,  bool gameHasStarted) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwDestroyWindow(window);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        // Toggle menu visibility when the Escape key is pressed
        returnToMenuClicked = !returnToMenuClicked;

        // You may want to add additional logic here, such as pausing the game
        // when the menu is visible and resuming when it's hidden.

        // Wait a short duration to prevent multiple toggles on a single key press
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    if(gameHasStarted){
        // Update car's position based on user input (e.g., keyboard or controller)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            carPosition += carFront * carSpeed * deltaTime;
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            carPosition -= carFront * carSpeed * deltaTime;
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            rotationAngle += carRotationSpeed * deltaTime;
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            rotationAngle -= carRotationSpeed * deltaTime;
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
    }
    else {
        if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)) {
            *chosenCarIndex = 1;
        }
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)) {
            *chosenCarIndex = 0;
        }
    }

    // Update car's front direction based on rotation
    carFront.x = sin(glm::radians(rotationAngle));
    carFront.z = -cos(glm::radians(rotationAngle));
    carFront = glm::normalize(carFront);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
//void mouse_callback(GLFWwindow *window, double xposd, double yposd) {
//    float xpos = static_cast<float>(xposd);
//    float ypos = static_cast<float>(yposd);
//    if (firstMouse) {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset =
//            lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//    lastX = xpos;
//    lastY = ypos;
//
//    camera.ProcessMouseMovement(xoffset, yoffset);
//}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}