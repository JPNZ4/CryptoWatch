#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>

#include "../lib/nlohmann/json.hpp"
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
    // HTTPS Get request to pull data from the Coincap API
    std::string CurlRequest(std::string requestString) const;
    // Get the price history for one coin over a time period
    void CoinHistoryRequest(std::string id, std::string interval, std::string start, std::string end);
    // Create CoinGainLoss struct data from CryptoCoinData
    void GenerateTopAndLowCoinGains();
    // Get coin data for all coins
    void GetAllCoinRequest();
    // Infinite loop requesting coin data at the set interval
    void PollingNetworkRequestStart(unsigned int interval);
    // Get XAxis data for CoinGainLoss
    std::vector<double> getXAxis() const { return _xAxis; }
    // Get YAxis data for CoinGainLoss
    std::vector<double> getYAxis() const { return _yAxis; }
    // Get all coin data
    std::vector<CoinData> GetCoinData()
    {
        const std::lock_guard<std::mutex> lock(_mutex);
        return _cryptoCoinsData;
    }
    // Get CoinGainLoss struct
    CoinGainLoss GetCoinGainLoss()
    {
        const std::lock_guard<std::mutex> lock(_mutex);
        return _coinsGainsAndLosses;
    }
    // Update CryptoCoinData
    void SetCoinData(std::vector<CoinData> cryptoCoinsData);
    // Update CoinGainLoss struct
    void SetCoinGainLoss(CoinGainLoss coinsGainsAndLosses);
    // Get a list of all coin names
    std::vector<std::string> GetCoinNamesList() const;

private:
    // Sort CrpytoCoinData by passed in property
    bool SortVector(const CoinData &a, const CoinData &b) const;
    // Format network request from JSON into a string
    std::string GetJSONValueString(nlohmann::json value) const;
    std::vector<double> _xAxis;
    std::vector<double> _yAxis;
    std::mutex _mutex;
    std::vector<CoinData> _cryptoCoinsData;
    CoinGainLoss _coinsGainsAndLosses;
};