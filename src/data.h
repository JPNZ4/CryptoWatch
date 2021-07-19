#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "../lib/nlohmann/json.hpp"
#include <chrono>
#include <thread>
#include <mutex>
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

struct CoinGainLoss
{
    const char *largestLabels[5] = {"", "", "", "", ""};
    float largestValues[5] = {0, 0, 0, 0, 0};
    const char *smallestLabels[5] = {"", "", "", "", ""};
    float smallestValues[5] = {0, 0, 0, 0, 0};
};

class Data
{
public:
    Data();
    std::string CurlRequest(std::string requestString) const;
    void CoinHistoryRequest(std::string id, std::string interval, std::string start, std::string end);
    void GenerateTopAndLowCoinGains();
    void GetAllCoinRequest();
    void PollingNetworkRequestStart(unsigned int interval);

    std::vector<double> getXAxis() const { return _xAxis; }
    std::vector<double> getYAxis() const { return _yAxis; }
    std::vector<CoinData> GetCoinData() { return _cryptoCoinsData; }
    CoinGainLoss GetCoinGainLoss() { return _coinsGainsAndLosses; }
    void SetCoinData(std::vector<CoinData> cryptoCoinsData);
    void SetCoinGainLoss(CoinGainLoss coinsGainsAndLosses);
    std::vector<std::string> GetCoinNamesList() const;
private:
    bool SortVector(const CoinData &a, const CoinData &b) const;
    std::string GetJSONValueString(nlohmann::json value) const;
    std::vector<double> _xAxis;
    std::vector<double> _yAxis;
    std::mutex _mutex;
    std::vector<CoinData> _cryptoCoinsData;
    CoinGainLoss _coinsGainsAndLosses;
};