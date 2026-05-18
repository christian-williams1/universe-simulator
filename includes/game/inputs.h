#pragma once

#include "../config.h"
#include "player.h"

class Inputs
{
    public:
        // constructor
        Inputs(GLFWwindow *window, int shader, Player &player);

        // user interaction
        void process_mouse(double xoffset, double yoffset);
        void process_input(float dt);

        glm::mat4 projection;
        
        glm::mat4 view;

    private:
        GLFWwindow *window;

        // sensitivities
        float speed;
        float sensitivity;

        // camera vectors
        //glm::dvec3 cameraPos = glm::dvec3(0.0f, 0.0f, 5.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

        // mouse movements
        float yaw = -90.0f;
        float pitch = 0.0f;
        float lastX;
        float lastY;
        float fov;

        Player *player;
};