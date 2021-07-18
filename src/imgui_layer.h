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

    void Start(std::pair<int, int> windowDimensions);
    void End();

    void CreateTableWidget(std::vector<CoinData> &CryptoCoinsData);
    void CreateLinePlotWidget(std::vector<double> xAxis, std::vector<double> yAxis);
    void CreateBarGraphWidget(const char* title, const char* labels[5], float values[5]);

private:
    int _windowWidth;
    int _windowHeight;
};