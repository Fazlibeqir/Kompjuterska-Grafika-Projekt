//
// Created by Elena on 08.7.2023.
//
#include <thread>
#include "init.h"
#include "GlobalVariables.h"

Init::Init(){}
GLFWwindow * Init:: initializeWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    char *appName = "Velocity Xtreme";
    GLFWwindow *window = glfwCreateWindow(GlobalVariables::scrWidth, GlobalVariables::scrHeight, appName, NULL, NULL);
    if (window == NULL) {
        std::cout << "ERROR: failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "ERROR: failed to initialise GLAD" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    return window;
}

void Init:: updateDeltaTime(){
    float currentFrame = glfwGetTime();
    GlobalVariables::deltaTime = currentFrame - GlobalVariables::lastFrame;
    GlobalVariables::lastFrame = currentFrame;
}

void Init:: mouseCallback(GLFWwindow *window, double xposd, double yposd){
    if (GlobalVariables::firstMouse) {
        GlobalVariables::lastX = xposd;
        GlobalVariables::lastY = yposd;
        GlobalVariables::firstMouse = FALSE;
    }

    float xoffset = xposd - GlobalVariables::lastX;
    float yoffset = GlobalVariables::lastY - yposd;

    GlobalVariables::lastX = xposd;
    GlobalVariables::lastY = yposd;

    if (!GlobalVariables::cameraFollow) {
        GlobalVariables::camera.ProcessMouseMovement(xoffset, yoffset);
    } else if (GlobalVariables::rotating) {
        GlobalVariables::baseYaw += xoffset;
        GlobalVariables::basePitch += yoffset;
        if (GlobalVariables::basePitch > 89.0f)
            GlobalVariables::basePitch = 89.0f;
        if (GlobalVariables::basePitch < -89.0f)
            GlobalVariables::basePitch = -89.0f;
        GlobalVariables::camera.ProcessMouseMovement(0, yoffset);
    }
}
void Init:: mouseButtonCallback(GLFWwindow *window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        GlobalVariables::rotating = TRUE;
    else
        GlobalVariables::rotating = FALSE;
}
void Init:: scrollCallback(GLFWwindow *window, double d, double y){
    GlobalVariables::cameraRadius -= y / 2;
    if (GlobalVariables::basePitch > 20.0f)
        GlobalVariables::basePitch = 20.0f;
    if (GlobalVariables::basePitch < 0.0f)
        GlobalVariables::basePitch = 0.0f;
}
void Init:: processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        // Toggle menu visibility when the Escape key is pressed
        GlobalVariables::returnToMenuClicked = !GlobalVariables::returnToMenuClicked;

        // You may want to add additional logic here, such as pausing the game
        // when the menu is visible and resuming when it's hidden.

        // Wait a short duration to prevent multiple toggles on a single key press
      //  std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwDestroyWindow(window);
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !GlobalVariables::switched) {
        GlobalVariables::cameraFollow = !GlobalVariables::cameraFollow;
        GlobalVariables::switched = TRUE;
        if (GlobalVariables::cameraFollow) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) {
        GlobalVariables::switched = FALSE;
    }

    if (!GlobalVariables::cameraFollow) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            GlobalVariables::camera.ProcessKeyboard(FORWARD, GlobalVariables::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            GlobalVariables::camera.ProcessKeyboard(BACKWARD, GlobalVariables::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            GlobalVariables::camera.ProcessKeyboard(LEFT, GlobalVariables::deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            GlobalVariables::camera.ProcessKeyboard(RIGHT, GlobalVariables::deltaTime);
    }

    float steering_limit = 1.0f;
    float steering_speed = 0.05f;

    // Car controls - steering
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (GlobalVariables::steering > -steering_limit)
            GlobalVariables::steering -= steering_speed;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (GlobalVariables::steering < steering_limit)
            GlobalVariables::steering += steering_speed;
    } else {
        GlobalVariables::steering -= steering_speed * ((GlobalVariables::steering > 0) - (GlobalVariables::steering < 0));
        if (GlobalVariables::steering < steering_speed && GlobalVariables::steering > -steering_speed)
            GlobalVariables::steering = 0.0f;
    }

    // Car controls - acceleration
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        GlobalVariables::acceleration = 1;
        //audio.pressed=!audio.pressed;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        GlobalVariables::acceleration = -1;
    } else {
        GlobalVariables::acceleration = 0;
        GlobalVariables::handbrake = TRUE;
    }

    // Car controls - handbrake
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        GlobalVariables::handbrake = TRUE;
    } else {
        GlobalVariables::handbrake = FALSE;
    }

    // Car controls - reset
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !GlobalVariables::gotUp) {
        GlobalVariables::getUp = TRUE;
        GlobalVariables::gotUp = TRUE;
    } else {
        GlobalVariables::getUp = FALSE;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
        GlobalVariables::gotUp = FALSE;
    }

    // Car controls - jump upwards
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !GlobalVariables::jumped) {
        GlobalVariables::jump = TRUE;
        GlobalVariables::jumped = TRUE;
    } else {
        GlobalVariables::jump = FALSE;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) {
        GlobalVariables::jumped = FALSE;
    }
}


map<const string, string> Init:: initializeShadersAndModelsPaths(){
    map<const string, string> returnMap;
    returnMap["carVertPath"]= string(SHADER_DIR)+"\\car.vert";
    returnMap["carFragPath"]= string(SHADER_DIR)+ "\\car.frag";
    returnMap["skyVertPath"]= string(SHADER_DIR)+"\\skybox.vert";
    returnMap["skyFragPath"]= string(SHADER_DIR)+"\\skybox.frag";
    returnMap["terrainVertPath"]= string(SHADER_DIR)+"\\terrian.vert";
    returnMap["terrainFragPath"]= string(SHADER_DIR)+"\\terrian.frag";
    returnMap["carModelPath"] =  string(MODEL_DIR)+"\\car\\car.obj";
    returnMap["tyre1ModelPath"] =  string(MODEL_DIR)+"\\car\\tyref.obj";
    returnMap["tyre2ModelPath"] =  string(MODEL_DIR)+"\\car\\tyreb.obj";
    returnMap["terrainModel1Path"] =  string(SKY_DIR)+"\\terrian\\grass.obj";
    returnMap["terrainModel2Path"] =  string(SKY_DIR)+"\\terrian\\roads.obj";
    returnMap["terrainModel3Path"] =  string(SKY_DIR)+"\\pre-game\\untitled.obj";

    return returnMap;
}
