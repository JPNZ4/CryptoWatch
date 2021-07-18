#pragma once

class CustomGLFWWindow {
    public:
        CustomGLFWWindow(int width, int height) : _width(width), _height(height) {};
        void CreateWindow();
        // window uses glfwCreateWindow which uses malloc, therefore not using a smart pointer.
        GLFWwindow *window = nullptr;
        void ClearWindow();
        bool WindowShouldClose();
        void EndOfRunLoopWindowFunctions();
        void DetachGLFWWindow();
        std::pair<int, int> GetWindowDimensions();
    private:
        int _width;
        int _height;
};