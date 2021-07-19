#pragma once

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_glfw.h"
#include "../lib/imgui/imgui_impl_opengl3.h"
#include "../lib/imgui/implot.h"
#include "data.h"
#include <string>
#include <vector>

enum CyrpocurrencyColumnID
{
    ColumnID_ID,
    ColumnID_Rank,
    ColumnID_Symbol,
    ColumnID_Name,
    ColumnID_Supply,
    ColumnID_MarketCapUsd,
    ColumnID_VolumeUsd24Hr,
    ColumnID_PriceUsd,
    ColumnID_ChangePercent24Hr,
};

class CustomGLFWWindow;

class ImGuiLayer
{
public:
    ImGuiLayer(CustomGLFWWindow &customGLFWWindow);
    // Creates new OpenGL and ImGui frame, also sets window dimensions
    void Start(std::pair<int, int> windowDimensions);
    // Runs the render and draw commands
    void End() const;
    // Create a table widget that displays columns of Crpyto Currency data
    void CreateTableWidget(std::vector<CoinData> CryptoCoinsData) const;
    // Create a line plot which displays one Crypto Currency coin value since the start of 2021
    void CreateLinePlotWidget(Data &data) const;
    // Creates a bar graph widget which displays 5 bars
    void CreateBarGraphWidget(const char* title, const char* labels[5], float values[5], std::pair<float, float> startPos, std::pair<float, float> size) const;
private:
    int _windowWidth;
    int _windowHeight;
};