#include "imgui_layer.h"
#include "custom_glfw_window.h"
#include <string>

ImGuiLayer::ImGuiLayer(CustomGLFWWindow &customGLFWWindow)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImPlot::CreateContext();

    std::string glsl_version = "#version 150";
    ImGui_ImplGlfw_InitForOpenGL(customGLFWWindow.window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}

void ImGuiLayer::Start()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}