#include "imgui_layer.h"
#include "custom_glfw_window.h"

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

void ImGuiLayer::CreateTableWidget(std::vector<CoinData> &CryptoCoinsData)
{

    static ImGuiTableFlags flags =
        ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY;
    // Using those as a base value to create width/height that are factor of the size of our font
    const float TEXT_BASE_WIDTH = ImGui::CalcTextSize("A").x;
    const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
    if (ImGui::BeginTable("Cryptocurrency Prices", 8, flags, ImVec2(0.0f, TEXT_BASE_HEIGHT * 15), 0.0f))
    {
        // Declare columns
        // We use the "user_id" parameter of TableSetupColumn() to specify a user id that will be stored in the sort specifications.
        // This is so our sort function can identify a column given our own identifier. We could also identify them based on their index!
        // Demonstrate using a mixture of flags among available sort-related flags:
        ImGui::TableSetupColumn("Rank", ImGuiTableColumnFlags_WidthFixed, 0.0f, ColumnID_Rank);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_Name);
        ImGui::TableSetupColumn("Symbol", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, ColumnID_Symbol);
        ImGui::TableSetupColumn("Price", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_PriceUsd);
        ImGui::TableSetupColumn("24Hr %", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_ChangePercent24Hr);
        ImGui::TableSetupColumn("Supply", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_Supply);
        ImGui::TableSetupColumn("Volume (24hr)", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_VolumeUsd24Hr);
        ImGui::TableSetupColumn("Market Cap", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, ColumnID_MarketCapUsd);
        ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
        ImGui::TableHeadersRow();

        // Sort our data if sort specs have been changed!
        // if (ImGuiTableSortSpecs* sorts_specs = ImGui::TableGetSortSpecs())
        // if (sorts_specs->SpecsDirty)
        // {
        //     MyItem::s_current_sort_specs = sorts_specs; // Store in variable accessible by the sort function.
        //     if (items.Size > 1)
        //         qsort(&items[0], (size_t)items.Size, sizeof(items[0]), MyItem::CompareWithSortSpecs);
        //     MyItem::s_current_sort_specs = NULL;
        //     sorts_specs->SpecsDirty = false;
        // }

        // Demonstrate using clipper for large vertical lists
        ImGuiListClipper clipper;
        clipper.Begin(CryptoCoinsData.size());
        while (clipper.Step())
            for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
            {
                // Display a data item
                CoinData *item = &CryptoCoinsData[row_n];
                ImGui::PushID(std::stoi(item->rank));
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text(item->rank.c_str());
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(item->name.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(item->symbol.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(item->priceUsd.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(item->changePercent24Hr.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(item->supply.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(item->volumeUsd24Hr.c_str());
                ImGui::TableNextColumn();
                ImGui::Text(item->marketCapUsd.c_str());
                ImGui::PopID();
            }
        ImGui::EndTable();
    }
}

void ImGuiLayer::CreateLinePlotWidget(std::vector<double> xAxis, std::vector<double> yAxis)
{
    bool my_tool_active = true;
    ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
    ImGui::BulletText("Anti-aliasing can be enabled from the plot's context menu (see Help).");
    if (ImPlot::BeginPlot("Coin Value", "Date", "$USD"))
    {
        ImPlot::PlotLine("CoinName**", xAxis.data(), yAxis.data(), 194);
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        ImPlot::EndPlot();
    }
    ImGui::End();
}

void ImGuiLayer::CreateBarGraphWidget(const char* title, const char *labels[5], float values[5])
{
    ImGui::Begin(title);
    // Each Bar is data in array
    const double positions[] = {1, 2, 3, 4, 5};
    ImPlot::SetNextPlotTicksX(positions, 5, labels);
    if (ImPlot::BeginPlot(title))
    {
        ImPlot::PlotBars(title, values, 5);
        ImPlot::EndPlot();
    }
    ImGui::End();
}