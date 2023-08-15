//
// Created by Elena on 08.7.2023.
//

#include "init.h"

<<<<<<< Updated upstream
=======

float deltaTime = 1.0f;
float lastFrame = 0.0f;
float carSpeed = 5.0f;
float rotationAngle = 0.0f;
glm::vec3 carPosition = glm::vec3(0.0f, 700.0f, 0.0f);
glm::vec3 carFront = glm::vec3(0.0f, 0.0f, -1.0f);
float carRotationSpeed = 50.0f;
bool returnToMenuClicked= false;

>>>>>>> Stashed changes
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
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetScrollCallback(mWindow, scroll_callback);
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_CAPTURED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(1);
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return mWindow;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}