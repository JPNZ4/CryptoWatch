#pragma once

#include <iostream>
#include <curl/curl.h>

class Data 
{
public:
    Data();
    std::string CurlRequest(std::string requestString);

private:
    CURL *curl;
};