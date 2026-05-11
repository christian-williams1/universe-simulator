#include <iostream>
#include "../includes/config.h"
#include "../includes/render/shader.h"
#include "../includes/inputs/inputs.h"

//void processInput(GLFWwindow *window, int shader);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// keeping track of time betweeen each frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// defining mouse movement vars
Inputs *glblInputs = nullptr;
bool firstMouse = true;
float lastX = cfg::winWidth/2.0f;
float lastY = cfg::winHeight/2.0f;

int main()
{
    // initialising window
    GLFWwindow *window;

    if (!glfwInit())
    {
        std::cout << "Unable to open window" << std::endl;

        return -1;
    }

    window = glfwCreateWindow(700, 700, "program", NULL, NULL);

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

    std::vector<float> vertices = {
        -0.5f, -0.5f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   // top left
         0.5f, -0.5f, 0.0f,   // bottom right
         0.5f,  0.5f, 0.0f    // top right
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        1, 2, 3
    };

    unsigned int EBO;
    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void *)0);
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
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents(); // remove events from stack
    }

    glBindVertexArray(0);
    return 0;
}

// controlling rotational movement of the screen via mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
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