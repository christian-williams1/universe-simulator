#include <iostream>
#include "../includes/config.h"
#include "../includes/render/shader.h"
#include "../includes/inputs/inputs.h"
#include "../includes/render/cube_sphere.h"

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

    std::cout << cfg::winWidth << "\n";
    std::cout << cfg::winHeight << "\n";
    std::cout << "Refresh Rate: " << mode->refreshRate << std::endl;

    window = glfwCreateWindow(cfg::winWidth, cfg::winHeight, "program", NULL, NULL);
    glfwSetWindowPos(window, 0, 0);

    if (window == NULL)
    {
        std::cout << "Unable to load window" << std::endl;

        return -1;
    }

    // creating inputs class
    Inputs inputs(window, glm::vec3(0.0f, 0.0f, -3.0f));
    glblInputs = &inputs;

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glViewport(0, 0, cfg::winWidth, cfg::winHeight);

    // creating triangle
    Shader shader("../shaders/vertex.vert", "../shaders/fragment.frag");

    glEnable(GL_DEPTH_TEST);

    CubeSphere cubeSphere(20);

    unsigned int EBO;
    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeSphere.vertices.size() * sizeof(glm::vec3), cubeSphere.vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeSphere.indices.size() * sizeof(unsigned int), cubeSphere.indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shader.shaderID);
    glBindVertexArray(VAO);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        inputs.process_input(deltaTime, shader.shaderID);
        glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPointSize(10.0f);
        glDrawElements(GL_POINTS, cubeSphere.indices.size(), GL_UNSIGNED_INT, 0);

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