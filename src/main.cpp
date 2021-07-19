#include <iostream>
#include <vector>

#include <GLFW/glfw3.h>

#include "custom_glfw_window.h"
#include "data.h"
#include "imgui_layer.h"

int main()
{
    CustomGLFWWindow customGLFWWindow(1280, 620);
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

    while (!customGLFWWindow.WindowShouldClose())
    {
        customGLFWWindow.ClearWindow();
        
        ImGuiLayer.Start(customGLFWWindow.GetWindowDimensions());
        ImGuiLayer.CreateLinePlotWidget(data.getXAxis(), data.getYAxis());
        ImGuiLayer.CreateBarGraphWidget("Biggest Gains (24hrs)", coinsGainsAndLosses.largestLabels, coinsGainsAndLosses.largestValues, std::pair<float, float>(0.6666, 0), std::pair<float, float>(0.3333, 320));
        ImGuiLayer.CreateBarGraphWidget("Biggest Losses (24hrs)", coinsGainsAndLosses.smallestLabels, coinsGainsAndLosses.smallestValues, std::pair<float, float>(0.6666, 300), std::pair<float, float>(0.3333, 320));        
        ImGuiLayer.CreateTableWidget(CryptoCoinsData);
        ImGuiLayer.End();

        customGLFWWindow.EndOfRunLoopWindowFunctions();
    }

    customGLFWWindow.DetachGLFWWindow();
    return 0;
}