#include <iostream>
#include <vector>

#include <curl/curl.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_glfw.h"
#include "../lib/imgui/imgui_impl_opengl3.h"
#include "../lib/imgui/implot.h"
#include "../lib/nlohmann/json.hpp"

struct CoinData {
    std::string id;
    std::string rank;
    std::string symbol;
    std::string name;
    std::string supply;
    std::string marketCapUsd;
    std::string volumeUsd24Hr;
    std::string priceUsd;
    std::string changePercent24Hr;
};

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

size_t static curl_write(void *buffer, size_t size, size_t nmemb, void *userp)
{
    //  userp += strlen(userp);  // Skipping to first unpopulated char
     memcpy(userp, buffer, nmemb);  // Populating it.
     return nmemb;
}

std::size_t bf_callback(char* ptr, size_t size, size_t num, void* userdata)
{
    if(auto s = reinterpret_cast<std::string*>(userdata))
    {
        // only get here if userdata is not nullptr
        s->append(ptr, ptr + (size * num));
        return size * num;
    }

    return 0; // indicate error to framework
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

    std::string str_callback;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, "api.coincap.io/v2/assets");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &bf_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str_callback);
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);

    auto jsonCoinData = nlohmann::json::parse(str_callback);

    std::vector<CoinData> CryptoCoinsData;

    // Check there is an entry with data - which contains the array of coin data
    if (jsonCoinData.find("data") != jsonCoinData.end()) {
        for (auto& [key, value] : jsonCoinData.items()) {
            // Ensure the data contains an array
            if (value.is_array())
            {
                // Loop all elements (coin) of array
                for (auto& element : value) {
                    // Convert JSON Object into CoinData struct
                    CoinData coin {
                        element["id"].get<std::string>(),
                        element["rank"].get<std::string>(),
                        element["symbol"].get<std::string>(),
                        element["name"].get<std::string>(),
                        element["supply"].get<std::string>(),
                        element["marketCapUsd"].get<std::string>(),
                        element["volumeUsd24Hr"].get<std::string>(),
                        element["priceUsd"].get<std::string>(),
                        element["changePercent24Hr"].get<std::string>(),
                    };
                    // Push CoinData struct to array
                    CryptoCoinsData.push_back(coin);
                }
            }
        }
    }

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

        const ImVec2 cursorPos(0, 0);
        ImGui::SetNextWindowPos(cursorPos);
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
            const ImVec2 cursorPos(650, 0);
            ImGui::SetNextWindowPos(cursorPos);
            // Implot test example
            ImGui::Begin("Bar Window Test");
            if (ImPlot::BeginPlot("My Plot"))
            {
                ImPlot::PlotBars("My Bar Plot", bar_data, 11);
                ImPlot::EndPlot();
            }
            ImGui::End();
        }

        {
            ImGui::ShowDemoWindow();
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