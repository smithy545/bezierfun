//
// Created by Philip on 3/1/2020.
//

#ifndef PLACEHOLDER_RENDERUTIL_H
#define PLACEHOLDER_RENDERUTIL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>


class RenderUtil {
public:
    static GLuint loadShaderProgram(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);

    static GLuint loadTexture(const std::string &texturePath, int &width, int &height);
};


#endif //PLACEHOLDER_RENDERUTIL_H
