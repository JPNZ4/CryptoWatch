#include "data.h"

Data::Data()
{
    curl = curl_easy_init();
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
    CURLcode res;

    std::string str_callback;
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_URL, "api.coincap.io/v2/assets"); // Needs to be char*      
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &bf_callbackk);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &str_callback);
        struct curl_slist *headers = NULL;
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        res = curl_easy_perform(curl);
        std::cout << "Result: " << res << std::endl;
    }
    if(res != CURLE_OK)
    {
        throw std::runtime_error(curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
     std::cout << "Result String: " << str_callback << std::endl;
    return str_callback;
}