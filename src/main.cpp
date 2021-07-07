#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_glfw.h"
#include "../lib/imgui/imgui_impl_opengl3.h"
#include "../lib/imgui/implot.h"
#include <curl/curl.h>

GLFWwindow *initialize()
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

int main()
{
    GLFWwindow *window = initialize();
    if (!window)
    {
        return 0;
    }

    // Curl example getting basic api data
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, "api.coincap.io/v2/assets");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);

    // Set the clear color to a nice green
    glClearColor(0.15f, 0.6f, 0.4f, 1.0f);
    std::string glsl_version = "#version 150";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImPlot::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    // test vars
    bool my_tool_active = true;
    int bar_data[11] = {2, 3, 4, 5, 2, 7, 7, 9, 1, 0, 11};

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create a window called "My First Tool", with a menu bar.
        ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O"))
                { /* Do stuff */
                }
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                { /* Do stuff */
                }
                if (ImGui::MenuItem("Close", "Ctrl+W"))
                {
                    my_tool_active = false;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Plot some values
        const float my_values[] = {0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f};
        ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

        // Display contents in a scrolling region
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
        ImGui::BeginChild("Scrolling");
        for (int n = 0; n < 50; n++)
            ImGui::Text("%04d: Some text", n);
        ImGui::EndChild();
        ImGui::End();

        {
            // Implot test example
            ImGui::Begin("Bar Window Test");
            if (ImPlot::BeginPlot("My Plot"))
            {
                ImPlot::PlotBars("My Bar Plot", bar_data, 11);
                ImPlot::EndPlot();
            }
            ImGui::End();
        }

        // rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}