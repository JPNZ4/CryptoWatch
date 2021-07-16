#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "../lib/nlohmann/json.hpp"
#include <chrono>
#include <thread>
#include <curl/curl.h>

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

class Data 
{
public:
    Data();
    std::string CurlRequest(std::string requestString);
    void coinHistoryRequest(std::string id, std::string interval, std::string start, std::string end, std::vector<double> &xAxis, std::vector<double> &yAxis);
    void createBiggestGainsArray(std::vector<CoinData> CryptoCoinsData, CoinGainLoss &coinGainLoss);
    void networkCall(std::vector<CoinData> &CryptoCoinsData, CoinGainLoss &coinGainLoss);
    void timer_start(unsigned int interval, std::vector<CoinData> &CryptoCoinsData, CoinGainLoss &coinGainLoss);
private:
    bool sortVector(const CoinData& a, const CoinData& b);
    std::string getJSONValueString(nlohmann::json value);
};