#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "custom_glfw_window.h"

GLFWwindow* CustomGLFWWindow::CreateWindow()
{
    int glfwInitRes = glfwInit();
        if (!glfwInitRes)
        {
            std::cout << "Unable to initialize GLFW" << std::endl;
            return nullptr;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        GLFWwindow *window = glfwCreateWindow(1280, 720, "CryptoWatch", nullptr, nullptr);
        if (!window)
        {
            std::cout << "Unable to create GLFW window" << std::endl;
            glfwTerminate();
            return nullptr;
        }

        glfwMakeContextCurrent(window);
        int gladInitRes = gladLoadGL();
        if (!gladInitRes)
        {
            std::cout << "Unable to initialize glad" << std::endl;
            glfwDestroyWindow(window);
            glfwTerminate();
            return nullptr;
        }
        return window;
}