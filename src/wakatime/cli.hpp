#pragma once
#include <string>

namespace cli {
    void setResourcePath(const std::filesystem::path& resourcePath);
    std::filesystem::path getGlobalPath();
    std::filesystem::path getPath();
    bool isInstalled();
    std::string getCurrentVersion();
    void fetchLatestVersion();
    bool isLatest();

    std::string getCachedVersion();
    void cacheLatestVersion();
    
    void checkForDownloads();
    void onChecksFinished();

    bool execute(const std::filesystem::path& path, const std::vector<std::string>& args);

    std::string getVersion();
}