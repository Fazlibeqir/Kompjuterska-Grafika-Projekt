//
// Created by Elena on 08.7.2023.
//

#include <thread>
#include "init.h"




bool returnToMenuClicked= false;


//void processInput(GLFWwindow *window
//                 // ,bool gameHasStarted
//                  ) {
//    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
//        glfwDestroyWindow(window);
//    }
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
//        // Toggle menu visibility when the Escape key is pressed
//        returnToMenuClicked = !returnToMenuClicked;
//
//        // You may want to add additional logic here, such as pausing the game
//        // when the menu is visible and resuming when it's hidden.
//
//        // Wait a short duration to prevent multiple toggles on a single key press
//        std::this_thread::sleep_for(std::chrono::milliseconds(200));
//    }
////    if(gameHasStarted){
//        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !switched) {
//            cameraFollow = !cameraFollow;
//            switched = TRUE;
//            if (cameraFollow) {
//                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//            } else {
//                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//            }
//        }
//        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) {
//            switched = FALSE;
//        }
//
//        if (!cameraFollow) {
//            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//                camera.ProcessKeyboard(FORWARD, deltaTime);
//            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//                camera.ProcessKeyboard(BACKWARD, deltaTime);
//            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//                camera.ProcessKeyboard(LEFT, deltaTime);
//            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//                camera.ProcessKeyboard(RIGHT, deltaTime);
//        }
//
//        float steering_limit = 1.0f;
//        float steering_speed = 0.05f;
//
//        // Car controls - steering
//        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//            if (steering > -steering_limit)
//                steering -= steering_speed;
//        } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
//            if (steering < steering_limit)
//                steering += steering_speed;
//        } else {
//            steering -= steering_speed * ((steering > 0) - (steering < 0));
//            if (steering < steering_speed && steering > -steering_speed)
//                steering = 0.0f;
//        }
//
//        // Car controls - acceleration
//        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
//            acceleration = 1;
//        } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
//            acceleration = -1;
//        } else {
//            acceleration = 0;
//            handbrake = TRUE;
//        }
//
//        // Car controls - handbrake
//        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
//            handbrake = TRUE;
//        } else {
//            handbrake = FALSE;
//        }
//
//        // Car controls - reset
//        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !gotUp) {
//            getUp = TRUE;
//            gotUp = TRUE;
//        } else {
//            getUp = FALSE;
//        }
//        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
//            gotUp = FALSE;
//        }
//
//        // Car controls - jump upwards
//        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS && !jumped) {
//            jump = TRUE;
//            jumped = TRUE;
//        } else {
//            jump = FALSE;
//        }
//        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE) {
//            jumped = FALSE;
//        }
////    }
////    else {
////        if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)) {
////            *chosenCarIndex = 1;
////        }
////        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)) {
////            *chosenCarIndex = 0;
////        }
////    }
//}
//void mouseCallback(GLFWwindow *window, double xposd, double yposd) {
//    if (firstMouse) {
//        lastX = xposd;
//        lastY = yposd;
//        firstMouse = FALSE;
//    }
//
//    float xoffset = xposd - lastX;
//    float yoffset = lastY - yposd;
//
//    lastX = xposd;
//    lastY = yposd;
//
//    if (!cameraFollow) {
//        camera.ProcessMouseMovement(xoffset, yoffset);
//    } else if (rotating) {
//        baseYaw += xoffset;
//        basePitch += yoffset;
//        if (basePitch > 89.0f)
//            basePitch = 89.0f;
//        if (basePitch < -89.0f)
//            basePitch = -89.0f;
//        camera.ProcessMouseMovement(0, yoffset);
//    }
//}
//void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
//    cameraRadius -= yoffset / 2;
//    if (basePitch > 20.0f)
//        basePitch = 20.0f;
//    if (basePitch < 0.0f)
//        basePitch = 0.0f;
//}
//unsigned int loadCubeMap() {
//    string right = string(SKY_DIR) + "\\skybox\\right.png";
//    string left = string(SKY_DIR) + "\\skybox\\left.png";
//    string top = string(SKY_DIR) + "\\skybox\\top.png";
//    string bottom = string(SKY_DIR) + "\\skybox\\bottom.png";
//    string back = string(SKY_DIR) + "\\skybox\\back.png";
//    string front = string(SKY_DIR) + "\\skybox\\front.png";
//    unsigned int textureID;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//
//    int width, height, channels;
//    unsigned char *data;
//    std::vector<std::string> txt_faces;
//    txt_faces.push_back(right.c_str());
//    txt_faces.push_back(left.c_str());
//    txt_faces.push_back(top.c_str());
//    txt_faces.push_back(bottom.c_str());
//    txt_faces.push_back(front.c_str());
//    txt_faces.push_back(back.c_str());
//    for (unsigned int i = 0; i < 6; i++) {
//        data = stbi_load(txt_faces[i].c_str(), &width, &height, &channels, 0);
//        glTexImage2D(
//                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
//                width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
//        );
//        stbi_image_free(data);
//    }
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//
//    return textureID;
//}