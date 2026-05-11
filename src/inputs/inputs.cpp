#include "../includes/inputs/inputs.h"

Inputs::Inputs(GLFWwindow *window, glm::vec3 cameraPosition)
{
    this->window = window;

    // initialising camera/mouse
    this->cameraPos = cameraPosition;
    this->lastX = cfg::winWidth/2.0f;
    this->lastY = cfg::winHeight/2.0f;
}

void Inputs::process_input(float dt, int shader)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    
    const float cameraSpeed = cfg::speed*dt; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    glm::mat4 view;
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


    // before the cube loop
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(cfg::fov), cfg::winWidth/cfg::winHeight, 0.1f, 1000.0f);

    // upload once
    int viewLoc = glGetUniformLocation(shader, "view");
    int projLoc = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}


// controlling rotational movement of the screen via mouse
void Inputs::process_mouse(double xoffset, double yoffset)
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
    cameraFront = glm::normalize(direction);
}