#include "data.h"



Data::Data()
{
}

// All responses are string unless they are null, special function to handle the null cases
std::string Data::getJSONValueString(nlohmann::json value)
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

void Data::timer_start(unsigned int interval, std::vector<CoinData> &CryptoCoinsData, CoinGainLoss &coinGainLoss)
{
    std::thread([this, interval, &CryptoCoinsData, &coinGainLoss]() {
        while (true)
        {
            this->networkCall(CryptoCoinsData, coinGainLoss);
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }).detach();
}

bool Data::sortVector(const CoinData& a, const CoinData& b)
{
    return std::stod(a.changePercent24Hr) > std::stod(b.changePercent24Hr);
}

std::size_t bf_callbackk(char *ptr, size_t size, size_t num, void *userdata)
{
    if (auto s = reinterpret_cast<std::string *>(userdata))
    {
        // only get here if userdata is not nullptr
        s->append(ptr, ptr + (size * num));
        return size * num;
    }

    return 0; // indicate error to framework
}

std::string Data::CurlRequest(std::string requestString)
{
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    std::string str_callback;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, requestString.c_str()); // Needs to be char*      
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &bf_callbackk);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str_callback);
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
    }
    if(res != CURLE_OK)
    {
        throw std::runtime_error(curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
    return str_callback;
}

void Data::coinHistoryRequest(std::string id, std::string interval, std::string start, std::string end, std::vector<double> &xAxis, std::vector<double> &yAxis)
{
    std::string requestString = "api.coincap.io/v2/assets/" + id + "/history?interval=" + interval + "&start=" + start + "&end=" + end;
    auto jsonCoinData = nlohmann::json::parse(CurlRequest(requestString));

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

void Data::createBiggestGainsArray(std::vector<CoinData> CryptoCoinsData, CoinGainLoss &coinGainLoss)
{
    auto size = CryptoCoinsData.size() - 1;
    // std::sort(CryptoCoinsData.begin(), CryptoCoinsData.end(), Data::sortVector);
    std::sort(CryptoCoinsData.begin(), CryptoCoinsData.end(), [this](const CoinData& a, const CoinData& b){
        return this->sortVector(a, b);
    });
    for (int i = 0; i < 5; i++)
    {
        coinGainLoss.largestValues[i] = std::stof(CryptoCoinsData[i].changePercent24Hr);
        coinGainLoss.largestLabels[i] = CryptoCoinsData[i].symbol.c_str();
        coinGainLoss.smallestValues[i] = std::stof(CryptoCoinsData[size - i].changePercent24Hr);
        coinGainLoss.smallestLabels[i] = CryptoCoinsData[size - i].symbol.c_str();
    }
}

void Data::networkCall(std::vector<CoinData> &CryptoCoinsData, CoinGainLoss &coinGainLoss)
{
    auto jsonCoinData = nlohmann::json::parse(CurlRequest("api.coincap.io/v2/assets")); // Check for crash here? Happened once?

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
    // std::sort(CryptoCoinsData.begin(), CryptoCoinsData.end(), &Data::sortVector);
    std::sort(CryptoCoinsData.begin(), CryptoCoinsData.end(), [this](const CoinData& a, const CoinData& b){
        return this->sortVector(a, b);
    });
    if (CryptoCoinsData.size() > 0)
    {
        createBiggestGainsArray(CryptoCoinsData, coinGainLoss);
    }
}