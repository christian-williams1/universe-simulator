#include "../includes/game/inputs.h"

Inputs::Inputs(GLFWwindow *window, int shader, Player &player)
{
    this->window = window;

    this->player = &player;

    // initialising camera/mouse
    this->lastX = cfg::winWidth/2.0f;
    this->lastY = cfg::winHeight/2.0f;

    // setting perspective
    this->projection = glm::mat4(1.0f);
    this->projection = glm::perspective(glm::radians(cfg::fov), cfg::winWidth/cfg::winHeight, 0.1f, 10000.0f);

    this->keyDown = false;
}

void Inputs::process_input(float dt)
{
    //static int followCol = -1;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    
    const float cameraSpeed = cfg::speed*dt; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player->worldPos += cameraSpeed * cameraFront;  //  cameraSpeed * glm::vec3{0.0f, 0.0f, 1.0f};
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player->worldPos -= cameraSpeed * cameraFront;  //  cameraSpeed * glm::vec3{0.0f, 0.0f, 1.0f};
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player->worldPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player->worldPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        player->worldPos += cameraSpeed*cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        player->worldPos -= cameraSpeed*cameraUp;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !keyDown)
    {
        keyDown = true;
        glblState.timeScale *= 2;
    }
    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !keyDown)
    {
        keyDown = true;
        if (glblState.timeScale > 1) glblState.timeScale /= 2;
    }
    else if (
        glfwGetKey(window, GLFW_KEY_Q) != GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_E) != GLFW_PRESS &&
        keyDown)
    {
        keyDown = false;
    }
    this->view = glm::lookAt(glm::vec3{0.0f}, cameraFront, cameraUp);

    if (player->worldPos.x > 200)
    {
        this->view = glm::rotate(this->view, glm::radians(90.0f), glm::vec3(-1, 0, 0));
    }
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