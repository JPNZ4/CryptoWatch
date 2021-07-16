#include <iostream>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

#include <curl/curl.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "custom_glfw_window.h"
#include "data.h"
#include "imgui_layer.h"

int main()
{
    CustomGLFWWindow customGLFWWindow;
    customGLFWWindow.CreateWindow();

    // Catch error if window fails
    if (!customGLFWWindow.window)
    {
        return 0;
    }

    // TODO - Move these into Data class
    std::vector<CoinData> CryptoCoinsData;
    CoinGainLoss coinsGainsAndLosses;

    Data data;
    // Start loop to poll for Table CrptoCoin data
    data.timer_start(2000, CryptoCoinsData, coinsGainsAndLosses);
    data.coinHistoryRequest("bitcoin", "d1", "1609459200000", "1626308160000"); // Example to get single coin history

    // Create ImGui object
    ImGuiLayer ImGuiLayer(customGLFWWindow);

    while (!glfwWindowShouldClose(customGLFWWindow.window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiLayer.Start();

        ImGuiLayer.CreateLinePlotWidget(data.getXAxis(), data.getYAxis());
        ImGuiLayer.CreateBarGraphWidget("Biggest Gains", coinsGainsAndLosses.largestLabels, coinsGainsAndLosses.largestValues);
        ImGuiLayer.CreateBarGraphWidget("Biggest Losses", coinsGainsAndLosses.smallestLabels, coinsGainsAndLosses.smallestValues);        
        ImGuiLayer.CreateTableWidget(CryptoCoinsData);

        ImGuiLayer.End();

        glfwSwapBuffers(customGLFWWindow.window);
        glfwPollEvents();
    }

    // TODO - Move into glfwcustomwindow class destrcutor?
    glfwDestroyWindow(customGLFWWindow.window);
    glfwTerminate();

    return 0;
}