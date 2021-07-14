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
    memcpy(userp, buffer, nmemb); // Populating it.
    return nmemb;
}

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

std::vector<CoinData> networkCall()
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
    }
    curl_easy_cleanup(curl);

    auto jsonCoinData = nlohmann::json::parse(str_callback);

    std::vector<CoinData> CryptoCoinsData;

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
    return CryptoCoinsData;
}

int main()
{
    GLFWwindow *window = initialize();
    if (!window)
    {
        return 0;
    }

    std::vector<CoinData> CryptoCoinsData = networkCall();

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

        {
            // TODO - Delete - For Table reference
            // ImGui::ShowDemoWindow();
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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}