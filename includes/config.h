#pragma once
// config folder

// this folder will contain the config class and all dependencies to link

// *** Dependencies Folder Includes ***
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

// *** General Includes ***
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "math.h"

#include <vector>

// config class

namespace cfg {
    // win parameters
    inline float winWidth = 700.0f;
    inline float winHeight = 700.0f;

    // player parameters
    inline float speed = 90.0f;
    inline float sensitivity = 0.1f;
    inline float fov = 45.0f;

    // mathematical constants
    inline const double PI = 3.14159265359;
    inline const double G = 100.0f;
}