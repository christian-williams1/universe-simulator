#pragma once

#include "../includes/config.h"

class Shader 
{
    public:
        Shader(const std::string &vertex_filepath, const std::string &fragment_filepath);
        unsigned int shaderID;

    private:
        unsigned int make_shader_module(const std::string &filePath, unsigned int module_type);
};