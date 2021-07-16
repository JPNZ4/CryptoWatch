#pragma once

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_glfw.h"
#include "../lib/imgui/imgui_impl_opengl3.h"
#include "../lib/imgui/implot.h"

class CustomGLFWWindow;


class ImGuiLayer
{
public:
    ImGuiLayer(CustomGLFWWindow &customGLFWWindow);

    void Start();
    void End();
};