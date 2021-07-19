#include "data.h"



Data::Data()
{
}

// All responses are string unless they are null, special function to handle the null cases
std::string Data::GetJSONValueString(nlohmann::json value)
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

void Data::PollingNetworkRequestStart(unsigned int interval)
{
    std::thread([this, interval]() {
        while (true)
        {
            this->GetAllCoinRequest();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }).detach();
}

bool Data::SortVector(const CoinData& a, const CoinData& b)
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
    std::string callbackString;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, requestString.c_str()); // Needs to be char*      
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &bf_callbackk);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &callbackString);
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
    }
    if(res != CURLE_OK)
    {
        throw std::runtime_error(curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
    return callbackString;
}

void Data::CoinHistoryRequest(std::string id, std::string interval, std::string start, std::string end)
{
    std::string requestString = "api.coincap.io/v2/assets/" + id + "/history?interval=" + interval + "&start=" + start + "&end=" + end;
    auto jsonCoinData = nlohmann::json::parse(CurlRequest(requestString));

    if (_xAxis.size() > 0 && _yAxis.size() > 0) {
        _xAxis.clear();
        _yAxis.clear();
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
                    double price = std::stod(element["priceUsd"].get<std::string>());
                    double time = element["time"].get<double>() / 1000; // Convert to seconds for usage as time in ImPlot
                    _xAxis.push_back(time);
                    _yAxis.push_back(price);
                }
            }
        }
    }
}

void Data::GenerateTopAndLowCoinGains()
{
    auto size = _cryptoCoinsData.size() - 1;
    std::sort(_cryptoCoinsData.begin(), _cryptoCoinsData.end(), [this](const CoinData& a, const CoinData& b){
        return this->SortVector(a, b);
    });
    for (int i = 0; i < 5; i++)
    {
        _coinsGainsAndLosses.largestValues[i] = std::stof(_cryptoCoinsData[i].changePercent24Hr);
        _coinsGainsAndLosses.largestLabels[i] = _cryptoCoinsData[i].symbol.c_str();
        _coinsGainsAndLosses.smallestValues[i] = std::stof(_cryptoCoinsData[size - i].changePercent24Hr);
        _coinsGainsAndLosses.smallestLabels[i] = _cryptoCoinsData[size - i].symbol.c_str();
    }
}

void Data::GetAllCoinRequest()
{
    std::string apiResult = CurlRequest("api.coincap.io/v2/assets");
    auto jsonCoinData = nlohmann::json::parse(apiResult);

    const std::lock_guard<std::mutex> lock(_mutex);
    if (_cryptoCoinsData.size() > 0)
    {
        _cryptoCoinsData.clear();
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
                        GetJSONValueString(element["id"]),
                        GetJSONValueString(element["rank"]),
                        GetJSONValueString(element["symbol"]),
                        GetJSONValueString(element["name"]),
                        GetJSONValueString(element["supply"]),
                        GetJSONValueString(element["maxSupply"]),
                        GetJSONValueString(element["marketCapUsd"]),
                        GetJSONValueString(element["volumeUsd24Hr"]),
                        GetJSONValueString(element["priceUsd"]),
                        GetJSONValueString(element["changePercent24Hr"]),
                        GetJSONValueString(element["vwap24Hr"]),
                    };
                    // Push CoinData struct to array
                    _cryptoCoinsData.push_back(coin);
                }
            }
        }
    }
    if (_cryptoCoinsData.size() > 0)
    {
        GenerateTopAndLowCoinGains();
    }
}

std::vector<std::string> Data::GetCoinNamesList()
{
    std::vector<std::string> list;
    for (CoinData coin : _cryptoCoinsData) 
    {
        list.push_back(coin.id);
    }
    return list;
}