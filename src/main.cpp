#include <iostream>
#include "../includes/config.h"

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

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 700, 700);

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents(); // remove events from stack
    }
    return 0;
}