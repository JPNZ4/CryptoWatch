#pragma once

class CustomGLFWWindow {
    public:
        void CreateWindow();
        // window uses glfwCreateWindow which uses malloc, therefore not using a smart pointer.
        GLFWwindow *window = nullptr;
        void ClearWindow();
        bool WindowShouldClose();
        void EndOfRunLoopWindowFunctions();
        void DetachGLFWWindow();
};