#pragma once

class CustomGLFWWindow {
    public:
        CustomGLFWWindow(int width, int height) : _width(width), _height(height) {};
        // Initialize GLFW window
        void CreateWindow();
        // window uses glfwCreateWindow which uses malloc, therefore not using a smart pointer.
        GLFWwindow *window = nullptr;
        // Clear GLFW window each frame before rendering next frame
        void ClearWindow() const;
        // Check if window should close
        bool WindowShouldClose();
        // Swap buffers and poll events
        void EndOfRunLoopWindowFunctions();
        // Clean up window on exit
        void DetachGLFWWindow();
        // Get GLFW window dimensions (width x height)
        std::pair<int, int> GetWindowDimensions();
    private:
        int _width;
        int _height;
};