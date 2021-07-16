#include <iostream>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

#include <curl/curl.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_glfw.h"
#include "../lib/imgui/imgui_impl_opengl3.h"
#include "../lib/imgui/implot.h"
#include "../lib/nlohmann/json.hpp"

#include "custom_glfw_window.h"
#include "data.h"

struct CoinData
{
    std::string id;
    std::string rank;
    std::string symbol;
    std::string name;
    std::string supply;
    std::string maxSupply;
    std::string marketCapUsd;
    std::string volumeUsd24Hr;
    std::string priceUsd;
    std::string changePercent24Hr;
    std::string vwap24Hr;
};

struct CoinGainLoss {
    const char* largestLabels[5] = { "", "" , "", "", ""};
    float largestValues[5] = {0, 0, 0, 0, 0};
    const char* smallestLabels[5] = { "", "" , "", "", ""};;
    float smallestValues[5] = {0, 0, 0, 0, 0};
};

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

std::size_t bf_callback(char *ptr, size_t size, size_t num, void *userdata)
{
    if (auto s = reinterpret_cast<std::string *>(userdata))
    {
        // only get here if userdata is not nullptr
        s->append(ptr, ptr + (size * num));
        return size * num;
    }

    return 0; // indicate error to framework
}

// All responses are string unless they are null, special function to handle the null cases
std::string getJSONValueString(nlohmann::json value)
{
    if (value.is_null())
    {
        return "";
    }
    else
    {
        return value.get<std::string>();
    }
}

void timer_start(std::function<void (std::vector<CoinData>&, CoinGainLoss&)> func, unsigned int interval, std::vector<CoinData> &CryptoCoinsData, CoinGainLoss &coinGainLoss)
{
    std::thread([func, interval, &CryptoCoinsData, &coinGainLoss]() {
        while (true)
        {
            func(CryptoCoinsData, coinGainLoss);
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }).detach();
}

void coinHistoryRequest(std::string id, std::string interval, std::string start, std::string end, std::vector<double> &xAxis, std::vector<double> &yAxis)
{
    CURL *curl;
    CURLcode res;

    std::string str_callback;

    std::string requestString = "api.coincap.io/v2/assets/" + id + "/history?interval=" + interval + "&start=" + start + "&end=" + end;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, requestString.c_str()); // Needs to be char*        
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

    // Check there is an entry with data - which contains the array of coin data
    if (jsonCoinData.find("data") != jsonCoinData.end())
    {
        for (auto &[key, value] : jsonCoinData.items())
        {
            // Ensure the data contains an array
            if (value.is_array())
            {
                // Loop all elements (coin) of array
                for (auto &element : value)
                {
                    double price = std::stod(element["priceUsd"].get<std::string>());
                    double time = element["time"].get<double>() / 1000; // Convert to seconds for usage as time in ImPlot
                    xAxis.push_back(time);
                    yAxis.push_back(price);
                }
            }
        }
    }
}

bool sortVector(const CoinData& a, const CoinData& b)
{
    return std::stod(a.changePercent24Hr) > std::stod(b.changePercent24Hr);
}



void createBiggestGainsArray(std::vector<CoinData> CryptoCoinsData, CoinGainLoss &coinGainLoss)
{
    auto size = CryptoCoinsData.size() - 1;
    std::sort(CryptoCoinsData.begin(), CryptoCoinsData.end(), sortVector);
    for (int i = 0; i < 5; i++)
    {
        coinGainLoss.largestValues[i] = std::stof(CryptoCoinsData[i].changePercent24Hr);
        coinGainLoss.largestLabels[i] = CryptoCoinsData[i].symbol.c_str();
        coinGainLoss.smallestValues[i] = std::stof(CryptoCoinsData[size - i].changePercent24Hr);
        coinGainLoss.smallestLabels[i] = CryptoCoinsData[size - i].symbol.c_str();
    }
}

void networkCall(std::vector<CoinData> &CryptoCoinsData, CoinGainLoss &coinGainLoss)
{
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
        std::cout << "Result: " << res << std::endl;
    }
    curl_easy_cleanup(curl);
    
    auto jsonCoinData = nlohmann::json::parse(str_callback); // Check for crash here? Happened once?

    if (CryptoCoinsData.size() > 0)
    {
        CryptoCoinsData.clear();
    } 

    // Check there is an entry with data - which contains the array of coin data
    if (jsonCoinData.find("data") != jsonCoinData.end())
    {
        for (auto &[key, value] : jsonCoinData.items())
        {
            // Ensure the data contains an array
            if (value.is_array())
            {
                // Loop all elements (coin) of array
                for (auto &element : value)
                {
                    // Convert JSON Object into CoinData struct
                    CoinData coin{
                        getJSONValueString(element["id"]),
                        getJSONValueString(element["rank"]),
                        getJSONValueString(element["symbol"]),
                        getJSONValueString(element["name"]),
                        getJSONValueString(element["supply"]),
                        getJSONValueString(element["maxSupply"]),
                        getJSONValueString(element["marketCapUsd"]),
                        getJSONValueString(element["volumeUsd24Hr"]),
                        getJSONValueString(element["priceUsd"]),
                        getJSONValueString(element["changePercent24Hr"]),
                        getJSONValueString(element["vwap24Hr"]),
                    };
                    // Push CoinData struct to array
                    CryptoCoinsData.push_back(coin);
                }
            }
        }
    }
    std::sort(CryptoCoinsData.begin(), CryptoCoinsData.end(), sortVector);
    if (CryptoCoinsData.size() > 0)
    {
        createBiggestGainsArray(CryptoCoinsData, coinGainLoss);
    }
}







int main()
{
    CustomGLFWWindow customGLFWWindow;
    customGLFWWindow.CreateWindow();

    if (!customGLFWWindow.window)
    {
        return 0;
    }

    std::vector<double> xAxis;
    std::vector<double> yAxis;

    // coinHistoryRequest("bitcoin", "d1", "1609459200000", "1626308160000", xAxis, yAxis); // Example to get single coin history
    std::vector<CoinData> CryptoCoinsData;
    CoinGainLoss coinsGainsAndLosses;
    // Start network request in seperate thread to get data
    // timer_start(networkCall, 2000, CryptoCoinsData, coinsGainsAndLosses);

    Data data;
    auto test = data.CurlRequest("api.coincap.io/v2/assets");



    // Set the clear color to a nice greeny
    glClearColor(0.15f, 0.6f, 0.4f, 1.0f);
    std::string glsl_version = "#version 150";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    ImPlot::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(customGLFWWindow.window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    // test vars
    bool my_tool_active = true;
    int bar_data[11] = {2, 3, 4, 5, 2, 7, 7, 9, 1, 0, 11};

    while (!glfwWindowShouldClose(customGLFWWindow.window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            // TODO - Delete - For Table reference
            // ImGui::ShowDemoWindow();
        }

        // {
        //     ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
        //     ImGui::BulletText("Anti-aliasing can be enabled from the plot's context menu (see Help).");
        //     if (ImPlot::BeginPlot("Coin Value", "Date", "$USD")) 
        //     {
        //         ImPlot::PlotLine("CoinName**", xAxis.data(), yAxis.data(), 194);
        //         ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle);
        //         ImPlot::EndPlot();
        //     }
        //     ImGui::End();
        // }

        {
            ImGui::Begin("Bar Graph Test");
            // Each Bar is data in array
            const double positions[] = { 1, 2, 3, 4, 5};
            ImPlot::SetNextPlotTicksX(positions, 5, coinsGainsAndLosses.largestLabels);
            if (ImPlot::BeginPlot("Bar Plot")) {
                ImPlot::PlotBars("My Bar Plot", coinsGainsAndLosses.largestValues, 5);
                ImPlot::EndPlot();
            }
            ImGui::End();
        }

        {
             ImGui::Begin("Bar Graph Test2");
             const double positions[] = { 1, 2, 3, 4, 5};
            ImPlot::SetNextPlotTicksX(positions, 5, coinsGainsAndLosses.smallestLabels);
            if (ImPlot::BeginPlot("Bar Plot2")) {
                ImPlot::PlotBars("My Bar Plot2", coinsGainsAndLosses.smallestValues, 5);
                ImPlot::EndPlot();
            }
             ImGui::End();
        }

        

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
                // - ImGuiTableColumnFlags_DefaultSort
                // - ImGuiTableColumnFlags_NoSort / ImGuiTableColumnFlags_NoSortAscending / ImGuiTableColumnFlags_NoSortDescending
                // - ImGuiTableColumnFlags_PreferSortAscending / ImGuiTableColumnFlags_PreferSortDescending
                // ImGui::TableSetupColumn("Id", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, ColumnID_ID);
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

        // rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(customGLFWWindow.window);
        glfwPollEvents();
    }

    glfwDestroyWindow(customGLFWWindow.window);
    glfwTerminate();

    return 0;
}