#include "imgui_layer.h"
#include "custom_glfw_window.h"

ImGuiLayer::ImGuiLayer(CustomGLFWWindow &customGLFWWindow)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImPlot::CreateContext();

    std::string glslVersion = "#version 150";
    ImGui_ImplGlfw_InitForOpenGL(customGLFWWindow.window, true);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());
}

void ImGuiLayer::Start(std::pair<int, int> windowDimensions)
{
    _windowWidth = windowDimensions.first;
    _windowHeight = windowDimensions.second;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::CreateTableWidget(std::vector<CoinData> CryptoCoinsData)
{

    static ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY;
    // Using those as a base value to create width/height that are factor of the size of our font
    static ImGuiWindowFlags parentFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(_windowWidth * 0.6666, 300));
    ImGui::Begin("Cryptocurrency Statistics", (bool *)__null, parentFlags);
    if (ImGui::BeginTable("Cryptocurrency Prices", 8, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 15), 0.0f))
    {
        // Declare columns
        // We use the "user_id" parameter of TableSetupColumn() to specify a user id that will be stored in the sort specifications.
        // This is so our sort function can identify a column given our own identifier. We could also identify them based on their index!
        // Demonstrate using a mixture of flags among available sort-related flags:
        ImGui::TableSetupColumn("Rank", ImGuiTableColumnFlags_WidthFixed, 0.0f, ColumnID_Rank);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_Name);
        ImGui::TableSetupColumn("Symbol", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, ColumnID_Symbol);
        ImGui::TableSetupColumn("Price", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_PriceUsd);
        ImGui::TableSetupColumn("24Hr %", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_ChangePercent24Hr);
        ImGui::TableSetupColumn("Supply", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_Supply);
        ImGui::TableSetupColumn("Volume (24hr)", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_VolumeUsd24Hr);
        ImGui::TableSetupColumn("Market Cap", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_MarketCapUsd);
        ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
        ImGui::TableHeadersRow();

        // Sort our data if sort specs have been changed!
        if (ImGuiTableSortSpecs *sortSpecs = ImGui::TableGetSortSpecs())
            if (sortSpecs)
            {
                int direction = sortSpecs->Specs->SortDirection;
                int sortColumn = sortSpecs->Specs->ColumnIndex;
                std::sort(CryptoCoinsData.begin(), CryptoCoinsData.end(), [this, direction, sortColumn](const CoinData &a, const CoinData &b)
                          {
                              {
                                  switch (sortColumn)
                                  {
                                  case 0:
                                      if (direction == 1)
                                      {
                                          return std::stod(a.rank) < std::stod(b.rank);
                                      }
                                      else
                                      {
                                          return std::stod(a.rank) > std::stod(b.rank);
                                      }
                                      break;
                                  case 3:
                                      if (direction == 1)
                                      {
                                          return std::stod(a.priceUsd) < std::stod(b.priceUsd);
                                      }
                                      else
                                      {
                                          return std::stod(a.priceUsd) > std::stod(b.priceUsd);
                                      }
                                      break;
                                  case 4:
                                      if (direction == 1)
                                      {
                                          return std::stod(a.changePercent24Hr) < std::stod(b.changePercent24Hr);
                                      }
                                      else
                                      {
                                          return std::stod(a.changePercent24Hr) > std::stod(b.changePercent24Hr);
                                      }
                                      break;
                                  case 5:
                                      if (direction == 1)
                                      {
                                          return std::stod(a.supply) < std::stod(b.supply);
                                      }
                                      else
                                      {
                                          return std::stod(a.supply) > std::stod(b.supply);
                                      }
                                      break;
                                  case 6:
                                      if (direction == 1)
                                      {
                                          return std::stod(a.volumeUsd24Hr) < std::stod(b.volumeUsd24Hr);
                                      }
                                      else
                                      {
                                          return std::stod(a.volumeUsd24Hr) > std::stod(b.volumeUsd24Hr);
                                      }
                                      break;
                                  case 7:
                                      if (direction == 1)
                                      {
                                          return std::stod(a.marketCapUsd) < std::stod(b.marketCapUsd);
                                      }
                                      else
                                      {
                                          return std::stod(a.marketCapUsd) > std::stod(b.marketCapUsd);
                                      }
                                      break;

                                  default:
                                      if (direction == 1)
                                      {
                                          return std::stod(a.rank) < std::stod(b.rank);
                                      }
                                      else
                                      {
                                          return std::stod(a.rank) > std::stod(b.rank);
                                      }
                                      break;
                                  }
                              }
                          });
            }

        // Demonstrate using clipper for large vertical lists
        ImGuiListClipper clipper;
        clipper.Begin(CryptoCoinsData.size());
        while (clipper.Step())
            for (int rowN = clipper.DisplayStart; rowN < clipper.DisplayEnd; rowN++)
            {
                // Display a data item
                CoinData *item = &CryptoCoinsData[rowN];
                ImGui::PushID(std::stoi(item->rank));
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text(item->rank.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(item->name.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(item->symbol.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(std::stof(item->priceUsd) < 10 ? "%.7f" : "%.2f", std::stof(item->priceUsd));
                ImGui::TableNextColumn();
                ImGui::Text("%.5f", std::stof(item->changePercent24Hr));
                ImGui::TableNextColumn();
                ImGui::Text("%.0f", std::stof(item->supply));
                ImGui::TableNextColumn();
                ImGui::Text("%.0f", std::stof(item->volumeUsd24Hr));
                ImGui::TableNextColumn();
                ImGui::Text("%.0f", std::stof(item->marketCapUsd));
                ImGui::PopID();
            }
        ImGui::EndTable();
    }
    ImGui::End();
}

void ImGuiLayer::CreateLinePlotWidget(Data &data)
{
    std::vector<std::string> coinList = data.GetCoinNamesList();
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    ImGui::SetNextWindowPos(ImVec2(0, 300));
    ImGui::SetNextWindowSize(ImVec2(_windowWidth * 0.6666, 320));
    ImGui::Begin("Coin Plot", (bool *)__null, flags);
    const char* selectedCoin = "";
    if (coinList.size() > 0)
    {
        if (data.getXAxis().size() == 0) {
            // List generated - Generate list from first coin in lsit
            data.CoinHistoryRequest(coinList[0], "d1", "1609459200000", "1626308160000");
        }
        static int item_current_idx = 0;                              // Here we store our selection data as an index.
        selectedCoin = coinList[item_current_idx].c_str(); // Label to preview before opening the combo (technically it could be anything)
        if (ImGui::BeginCombo("Select Coin", selectedCoin))
        {
            for (int n = 0; n < coinList.size(); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(coinList[n].c_str(), is_selected))
                {
                    item_current_idx = n;
                    data.CoinHistoryRequest(coinList[item_current_idx], "d1", "1609459200000", "1626308160000");
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    static ImPlotAxisFlags xAxisPlotFlags = ImPlotAxisFlags_Time | ImPlotAxisFlags_AutoFit;
    static ImPlotAxisFlags yAxisPlotFlags =  ImPlotAxisFlags_AutoFit;
    if (ImPlot::BeginPlot("Coin Value Since Start of 2021", "Date", "$USD", ImVec2(-1, 0), 0, xAxisPlotFlags, yAxisPlotFlags))
    {
        ImPlot::PlotLine(selectedCoin, data.getXAxis().data(), data.getYAxis().data(), data.getXAxis().size());
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        ImPlot::EndPlot();
    }
    ImGui::End();
}

void ImGuiLayer::CreateBarGraphWidget(const char *title, const char *labels[5], float values[5], std::pair<float, float> startPos, std::pair<float, float> size)
{
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
    ImGui::SetNextWindowPos(ImVec2(startPos.first * _windowWidth, startPos.second));
    ImGui::SetNextWindowSize(ImVec2(_windowWidth * size.first, size.second));
    ImGui::Begin(title, (bool *)__null, flags);
    // Each Bar is data in array
    const double positions[] = {0, 1, 2, 3, 4};
    ImPlot::SetNextPlotTicksX(positions, 5, labels);
    static ImPlotFlags plotFlags = ImPlotFlags_NoLegend;
    if (ImPlot::BeginPlot(title, (const char *)__null, (const char *)__null, ImVec2(-1, 0), plotFlags, ImPlotAxisFlags_AutoFit, ImPlotAxisFlags_AutoFit))
    {
        ImPlot::PlotBars(title, values, 5);
        ImPlot::EndPlot();
    }
    ImGui::End();
}