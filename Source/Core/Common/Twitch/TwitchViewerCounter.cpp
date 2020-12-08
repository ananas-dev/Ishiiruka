#include "Common/StringUtil.h"
#include "Common/Twitch/TwitchViewerCounter.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <thread>
#include <curl/curl.h>
#include <json.hpp>

using json = nlohmann::json;

TwitchViewerCounter::TwitchViewerCounter()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);

}

TwitchViewerCounter::~TwitchViewerCounter()
{
    curl_global_cleanup();
}

size_t TwitchViewerCounter::WriteCallback(void* ptr, size_t size, size_t nmemb, std::string* data)
{
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

void TwitchViewerCounter::SetStreamer(std::string stream_url)
{
    if (!stream_url.empty())
    {
        // Search for the substring in string
        size_t pos = stream_url.find(m_twitch_domain);
        if (pos != std::string::npos)
        {
            // If found then erase it from string
            stream_url.erase(pos, m_twitch_domain.length());
        }
    }
    m_streamer = stream_url;
}

std::string TwitchViewerCounter::ApiRequest(std::string user_login)
{
    auto curl = curl_easy_init();
    std::string api_url = "https://api.twitch.tv/helix/streams?user_login=";
    const char* client_id = "0an8lvuwxp03ort4d5ghgn1r90j8as";
    const char* access_token = "djy8w6sah8c5uvbgvw4sl8tviuxyvd";
    std::string result_string;

    if (curl && !user_login.empty())
    {
        // Set the url
        curl_easy_setopt(curl, CURLOPT_URL, StringFromFormat("%s%s", api_url.c_str(), user_login.c_str()).c_str());

        // Set the headers
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, StringFromFormat("Client-Id: %s", client_id).c_str());
        headers = curl_slist_append(headers, StringFromFormat("Authorization: Bearer %s", access_token).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 

        // Get result
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result_string);

        // Perform the request
        curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
    curl = NULL;
    return result_string;
}

unsigned int TwitchViewerCounter::ParseResponse(std::string response_string) {
    unsigned int viewers = 0;

    if (!response_string.empty())
    {
        json response_json = json::parse(response_string)["data"][0];
        if (response_json.find("viewer_count") != response_json.end())
        {
            viewers = response_json["viewer_count"];
        }
    }
    return viewers;
}

void TwitchViewerCounter::ParallelProcess(bool running)
{
    if (running && !m_running)
    {
        m_running = true;

        side_process = std::thread([this]{
            while (m_running)
            {
                m_viewers = ParseResponse(ApiRequest(m_streamer));
                sleep(10);
            }
        });
        side_process.detach();
    }
    if (!running && m_running)
    {
        m_running = false;
        side_process.join(); 
    }
}