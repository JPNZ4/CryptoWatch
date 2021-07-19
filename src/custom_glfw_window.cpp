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

    window = glfwCreateWindow(_width, _height, "CryptoWatch", nullptr, nullptr);
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

void CustomGLFWWindow::ClearWindow() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

bool CustomGLFWWindow::WindowShouldClose()
{
    return glfwWindowShouldClose(window);
}

void CustomGLFWWindow::EndOfRunLoopWindowFunctions()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void CustomGLFWWindow::DetachGLFWWindow()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

std::pair<int, int> CustomGLFWWindow::GetWindowDimensions()
{
    std::pair<int, int> widthHeight;
    glfwGetWindowSize(window, &widthHeight.first, &widthHeight.second);
    _width = widthHeight.first;
    _height = widthHeight.second;
    return widthHeight;
}