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
#include "../includes/render/bloom.h"

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
    Shader planetShader("../shaders/vertex.vert", "../shaders/planet.frag");
    Shader sunShader("../shaders/vertex.vert", "../shaders/sun.frag");

    Shader ppBloom("../shaders/bloom.vert", "../shaders/bloom.frag"); // swapped bloom and blur add to video
    Shader ppBlur("../shaders/bloom.vert", "../shaders/blur.frag");

    // idk
    Shader shaders[2] = {planetShader, sunShader};

    glEnable(GL_DEPTH_TEST);

    // generating sphere
    CubeSphere cubeSphere(20);

    // creating planets
    Body *sun = new Body(nullptr, glm::vec3{1.0f, 0.9f, 0.3f}, {0, 0, 0, 0, 0}, 5.0f);
    Body *planet = new Body(sun, glm::vec3{1.0f, 0.5f, 0.25f}, {0.0, 400.0, 0.0, 0.0, 0.0}, 30.0f);
    Body *moon = new Body(planet, glm::vec3{1.0f, 0.5f, 0.25f}, {0.05, 10.0, 0.0, 0.0, 0.0}, 0.9f);

    sun->children.push_back(planet);
    planet->children.push_back(moon);
    SphereRenderer *daSun = new SphereRenderer(cubeSphere.vertices, cubeSphere.indices, sunShader);
    SphereRenderer *daPlanet = new SphereRenderer(cubeSphere.vertices, cubeSphere.indices, planetShader);
    SphereRenderer *daMoon = new SphereRenderer(cubeSphere.vertices, cubeSphere.indices, planetShader);

    // creating inputs class
    Player *player = new Player(*sun);
    Player *debug = new Player(*sun);
    Inputs inputs(window, planetShader.shaderID, *player);
    glblInputs = &inputs;

    // creating bloom effect class
    Bloom *bloom = new Bloom(ppBloom, ppBlur);

    glUseProgram(planetShader.shaderID);

    // enable face culling
    // glEnable(GL_CULL_FACE);  
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);

    while (!glfwWindowShouldClose(window))
    {
        // getting time between frames
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // processing any inputs on the stack to update view matrix
        inputs.process_input(deltaTime);

        // clearing default buffer
        glClearColor(0.00f, 0.00f, 0.00f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // rendering to FBO
        glBindFramebuffer(GL_FRAMEBUFFER, bloom->FBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // send updated inputs into respective shader programs
        for (auto &shader : shaders)
        {
            glUseProgram(shader.shaderID); // remove

            int projLoc = glGetUniformLocation(shader.shaderID, "projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(inputs.projection));

            int viewLoc = glGetUniformLocation(shader.shaderID, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(inputs.view));
        }

        // update planet position
        glm::dvec3 sunPos = glm::dvec3{0.0f};
        sun->orbit_traverse(sunPos);

        // rendering light container (sun)
        glUseProgram(sunShader.shaderID);
        daSun->draw(*sun, player->worldPos);

        // rendering containers (planets)
        glUseProgram(planetShader.shaderID);
        daPlanet->draw(*planet, player->worldPos);
        daMoon->draw(*moon, player->worldPos);

        // final rendering to quad
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(ppBlur.shaderID);
        bloom->draw_quad();

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