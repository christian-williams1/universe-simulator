#include <iostream>
#include "../includes/config.h"
#include "../includes/render/shader.h"
#include "../includes/game/inputs.h"
#include "../includes/render/cube_sphere.h"
#include "../includes/render/sphere_render.h"
#include "../includes/render/cube_map.h"
#include "../includes/simulation/body.h"
#include "../includes/simulation/orbit.h"
#include "../includes/game/player.h"

// void processInput(GLFWwindow *window, int shader);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

// keeping track of time betweeen each frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// defining mouse movement vars
Inputs *glblInputs = nullptr;
bool firstMouse = true;
float lastX = cfg::winWidth / 2.0f;
float lastY = cfg::winHeight / 2.0f;

// struct Body {
//     glm::vec3 position;
//     glm::vec3 velocity;
//};

int main()
{

    if (!glfwInit())
    {
        std::cout << "Unable to open window" << std::endl;

        return -1;
    }

    // setting to version 430
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // initialising window and monitor
    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    GLFWwindow *window;
    const GLFWvidmode *mode = glfwGetVideoMode(primary);

    if (primary == NULL)
    {
        std::cout << "Unable to fetch primary monitor" << std::endl;

        return -1;
    }

    if (mode == NULL)
    {
        std::cout << "Unable to initialise GLFWvidmode" << std::endl;

        return -1;
    }

    glfwWindowHint(GLFW_REFRESH_RATE, 60);
    cfg::winWidth = static_cast<float>(mode->width);
    cfg::winHeight = static_cast<float>(mode->height);

    window = glfwCreateWindow(cfg::winWidth, cfg::winHeight, "program", NULL, NULL);
    glfwSetWindowPos(window, 0, 0);

    if (window == NULL)
    {
        std::cout << "Unable to load window" << std::endl;

        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, cfg::winWidth, cfg::winHeight);

    // compiling shaders
    Shader shader("../shaders/vertex.vert", "../shaders/fragment.frag");
    Shader skyboxShader("../shaders/skybox_vert.vert", "../shaders/skybox_frag.frag");

    // creating skybox
    //SkyBox *skybox = new SkyBox();

    glEnable(GL_DEPTH_TEST);

    // generating sphere
    CubeSphere cubeSphere(20);

    // creating planets
    // KeplerianElements sunElements = {0, 0, 0, 0, 0};
    Body *sun = new Body(nullptr, glm::vec3{1.0f, 1.0f, 1.0f}, {0, 0, 0, 0, 0}, 5.0f);
    Body *planet = new Body(sun, glm::vec3{1.0f, 0.5f, 0.25f}, {0.0, 20000.0, 0.0, 0.0, 0.0}, 2000.0f);
    Body *moon = new Body(planet, glm::vec3{1.0f, 0.5f, 0.25f}, {0.05, 5.0, 0.0, 0.0, 0.0}, 0.9f);

    sun->children.push_back(planet);
    planet->children.push_back(moon);
    SphereRenderer *daSun = new SphereRenderer(cubeSphere.vertices, cubeSphere.indices);
    SphereRenderer *daPlanet = new SphereRenderer(cubeSphere.vertices, cubeSphere.indices);
    SphereRenderer *daMoon = new SphereRenderer(cubeSphere.vertices, cubeSphere.indices);

    // creating inputs class
    Player *player = new Player(*sun);
    Player *debug = new Player(*sun);
    Inputs inputs(window, shader.shaderID, *player);
    glblInputs = &inputs;

    glUseProgram(shader.shaderID);

    int perished[2] = {4, 4};
    bool collision = true;

    while (!glfwWindowShouldClose(window))
    {
        // getting time between frames
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // processing any inputs on the stack to update view matrix
        inputs.process_input(deltaTime, shader.shaderID);
        inputs.process_input(deltaTime, shader.shaderID); // updating general

        glClearColor(0.01f, 0.01f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //skybox->draw(skyboxShader, inputs.projection, inputs.view);

        // update planet position
        glm::dvec3 sunPos = glm::dvec3{0.0f};
        sun->orbit_traverse(sunPos);
        // std::cout << glm::length(planet->orbit->next_position()) << std::endl;

        daSun->draw(shader, *sun, player->worldPos);
        daPlanet->draw(shader, *planet, player->worldPos);
        daMoon->draw(shader, *moon, player->worldPos);

        glfwSwapBuffers(window);
        glfwPollEvents(); // remove events from stack
    }

    glBindVertexArray(0);
    glfwDestroyWindow(window);
    return 0;
}

// controlling rotational movement of the screen via mouse
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
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

    glblInputs->process_mouse(xoffset, yoffset);
}