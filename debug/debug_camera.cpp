#include "debug_camera.h"

DebugCamera::DebugCamera(GLFWwindow *window)
{
    this->window = window;
    this->projection = glm::perspective(glm::radians(cfg::fov), cfg::winWidth/cfg::winHeight, 0.1f, 100000.0f);
}

void DebugCamera::update_camera(float dt)
{
    //static int followCol = -1;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    
    const float cameraSpeed = cfg::speed*dt; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;  //  cameraSpeed * glm::vec3{0.0f, 0.0f, 1.0f};
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;  //  cameraSpeed * glm::vec3{0.0f, 0.0f, 1.0f};
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        cameraPos += cameraSpeed*cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        cameraPos -= cameraSpeed*cameraUp;
    }
    
    this->view = glm::lookAt(cameraPos, glm::vec3{0.0f}, cameraUp);
}

// controlling rotational movement of the screen via mouse
void DebugCamera::process_mouse(double xoffset, double yoffset)
{
    xoffset *= cfg::sensitivity;
    yoffset *= cfg::sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraPos = -glm::normalize(direction) * orbitRadius;
}

void DebugCamera::process_scroll(double yoffset)
{
    orbitRadius -= static_cast<float>(yoffset);

    orbitRadius = glm::clamp(orbitRadius, 2.0f, 1000.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraPos = -glm::normalize(direction) * orbitRadius;
}