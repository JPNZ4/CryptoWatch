#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "custom_glfw_window.h"

void CustomGLFWWindow::CreateWindow()
{
    int glfwInitRes = glfwInit();
        if (!glfwInitRes)
        {
            std::cout << "Unable to initialize GLFW" << std::endl;
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        window = glfwCreateWindow(1280, 720, "CryptoWatch", nullptr, nullptr);
        if (!window)
        {
            std::cout << "Unable to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);
        int gladInitRes = gladLoadGL();
        if (!gladInitRes)
        {
            std::cout << "Unable to initialize glad" << std::endl;
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }
}