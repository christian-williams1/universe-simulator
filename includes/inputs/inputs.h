#pragma once

#include "../config.h"

class Inputs
{
    public:
        // constructor
        Inputs(GLFWwindow *window, glm::vec3 cameraPosition, float x, float y, float fov);

        // user interaction
        void mouse_callback(double xpos, double ypos);
        void process_input(float dt);

    private:
        GLFWwindow *window;

        // camera vectors
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

        // mouse movements
        float yaw = -90.0f;
        float pitch = 0.0f;
        float lastX;
        float lastY;
        float fov;
};