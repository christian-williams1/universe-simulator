#pragma once

#include "../config.h"

// TEMP
struct Body {
    glm::vec3 position;
    glm::vec3 velocity;
};

class Inputs
{
    public:
        // constructor
        Inputs(GLFWwindow *window, glm::vec3 cameraPosition, int shader);

        // user interaction
        void process_mouse(double xoffset, double yoffset);
        void process_input(float dt, int shader, std::vector<Body> bodies);

        glm::mat4 projection;
        
        glm::mat4 view;

    private:
        GLFWwindow *window;

        // sensitivities
        float speed;
        float sensitivity;

        // camera vectors
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

        // mouse movements
        float yaw = -90.0f;
        float pitch = 0.0f;
        float lastX;
        float lastY;
        float fov;
};