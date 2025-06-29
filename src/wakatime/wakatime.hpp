#pragma once
#include <string>
#include <chrono>

namespace wakatime {
    void init();
    void refetch();

    void startHeartbeats();
    void stopHeartbeats();
    void sendHeartbeatsWrapper();
    void sendHeartbeat();

    std::filesystem::path getHomeDirectory();

    std::chrono::system_clock::time_point getLastHeartbeat();
}