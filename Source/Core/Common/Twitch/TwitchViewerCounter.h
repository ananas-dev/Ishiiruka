#pragma once

class TwitchViewerCounter
{
public:
    TwitchViewerCounter();
    ~TwitchViewerCounter();

    // Set the streamer
    void SetStreamer(std::string stream_url);

    // Get the response from the twitch api
    static std::string ApiRequest(std::string user_login);

    // Parse response
    static unsigned int ParseResponse(std::string response);

    // Side Process
    void StartOrContinueSideProcess();
    void StopSideProcess();

    unsigned int GetTwitchViewers() const
    {
        return m_viewers;
    }

private:
    static size_t WriteCallback(void* ptr, size_t size, size_t nmemb, std::string* data);

    bool m_running = false;
    std::thread side_process;
    std::string m_streamer;
    std::string m_twitch_domain = "https://www.twitch.tv/";
    std::string m_response;
    unsigned int m_viewers = 0;
};