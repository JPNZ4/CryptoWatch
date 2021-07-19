#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>

#include "custom_glfw_window.h"
#include "data.h"
#include "imgui_layer.h"

int main()
{
    // Create GLFW window
    CustomGLFWWindow customGLFWWindow(1280, 640);
    customGLFWWindow.CreateWindow();

    // Catch error if window fails
    if (!customGLFWWindow.window)
    {
        return 0;
    }

    // Create Data object to get data
    Data data;
    // Start loop to poll for Table CrptoCoin data
    data.PollingNetworkRequestStart(5000);

    // Create ImGui object
    ImGuiLayer ImGuiLayer(customGLFWWindow);

    // Start GLFW loop
    while (!customGLFWWindow.WindowShouldClose())
    {
        // Clear window each frame
        customGLFWWindow.ClearWindow();
        // Create ImGui widgets
        ImGuiLayer.Start(customGLFWWindow.GetWindowDimensions());
        ImGuiLayer.CreateTableWidget(data.GetCoinData());
        ImGuiLayer.CreateLinePlotWidget(data);
        ImGuiLayer.CreateBarGraphWidget("Biggest Gains (24hrs)", data.GetCoinGainLoss().largestLabels, data.GetCoinGainLoss().largestValues, std::pair<float, float>(0.6666, 0), std::pair<float, float>(0.3333, 320));
        ImGuiLayer.CreateBarGraphWidget("Biggest Losses (24hrs)", data.GetCoinGainLoss().smallestLabels, data.GetCoinGainLoss().smallestValues, std::pair<float, float>(0.6666, 320), std::pair<float, float>(0.3333, 320));        
        ImGuiLayer.End();
        // Process events
        customGLFWWindow.EndOfRunLoopWindowFunctions();
    }
    // Clean up GLFW
    customGLFWWindow.DetachGLFWWindow();
    return 0;
}