#pragma once

#include "../includes/config.h"

class DebugCamera
{
    public:
        DebugCamera(GLFWwindow *window);
        void update_camera(float dt);
        void process_mouse(double xoffset, double yoffset);

        glm::mat4 projection;
        glm::mat4 view;

    private:
        GLFWwindow *window;

        glm::vec3 cameraPos = glm::vec3{0.0f, 10.0f, 10.0f};
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

        // mouse movements
        float yaw = -90.0f;
        float pitch = 0.0f;
        float lastX;
        float lastY;
        float fov;
};