#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include <string>
#include <filesystem>
#include <random>
#include <fmt/format.h>
#include "../utils/utils.hpp"
#include "../utils/which.hpp"
#include "../utils/semver.hpp"
#include "cli.hpp"
#include "wakatime.hpp"

#ifdef GEODE_IS_WINDOWS
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

#if defined(_M_IX86) || defined(__i386__)
const std::string arch = "386";
#elif defined(_M_ARM64) || defined(__aarch64__)
const std::string arch = "arm64";
#else
const std::string arch = "amd64";
#endif

#ifdef GEODE_IS_WINDOWS
const std::string os = "windows";
#elif defined(GEODE_IS_IOS) || defined(GEODE_IS_MACOS)
const std::string os = "darwin";
#elif defined(GEODE_IS_ANDROID)
const std::string os = "android";
#else
const std::string os = "linux";
#endif

namespace cli {
    const std::string WAKATIME_DOWNLOAD_URL = "https://github.com/wakatime/wakatime-cli/releases/latest/download/";
    const std::string WAKATIME_VERSION_URL = "https://api.github.com/repos/wakatime/wakatime-cli/releases/latest";

    static std::filesystem::path s_path;
    static std::filesystem::path s_globalPath;
    static std::filesystem::path s_resourcePath;
    static std::string s_latestVersion;
    static std::string s_currentVersion;
    
    static bool s_installed = false;
    static bool s_shouldInstall = true;

    geode::EventListener<geode::utils::web::WebTask> webListener;
    geode::EventListener<geode::utils::web::WebTask> downloadListener;

    void setResourcePath(const std::filesystem::path& resourcePath) {
        s_resourcePath = resourcePath;
    }

    std::filesystem::path getGlobalPath() {
        if (!s_globalPath.empty()) return s_globalPath;

        std::string path = which::which("wakatime-cli");

        if (!path.empty()) {
            s_globalPath = path;
            s_path = path;
            geode::log::info("Using wakatime-cli from PATH");
        }

        return path;
    }

    std::filesystem::path getPath() {
        if (!s_path.empty()) return s_path;

        if (!getGlobalPath().empty()) return s_path;

        std::string path = fmt::format("wakatime-cli-{}-{}", os, arch);
        #ifdef GEODE_IS_WINDOWS
            path += ".exe";
        #endif

        s_path = s_resourcePath / path;

        return path;
    }

    bool isInstalled() {
        return std::filesystem::exists(getPath());
    }

    std::string getCurrentVersion() {
        if (!isInstalled()) return "";

        std::filesystem::path path = getPath();

        std::string command = utils::quote(path.string()) + "--version";
        
        std::string result;
        char buffer[128];

        FILE* pipe = POPEN(command.c_str(), "r");
        if (!pipe) {
            geode::log::error("Unable to fetch wakatime-cli version");
            return "";
        }

        while (!feof(pipe)) {
            if (fgets(buffer, sizeof(buffer), pipe) != nullptr) result += buffer;
        }

        PCLOSE(pipe);

        result.erase(0, result.find_first_not_of(" \n\r\t"));
        result.erase(result.find_last_not_of(" \n\r\t") + 1);

        if (!result.empty() && result[0] == 'v') result.erase(0, 1);
        
        geode::log::info("wakatime-cli version: {}", result);

        if (!s_installed) s_currentVersion = result;

        return result;
    }

    void fetchLatestVersion() {
        // auto downloading disabled, skip the entire download flow
        if (!geode::Mod::get()->getSettingValue<bool>("auto-download") || !geode::Mod::get()->hasSavedValue("auto-download-prompt")) return onChecksFinished();

        std::string cached = getCachedVersion();
        if (!cached.empty()) {
            s_latestVersion = cached;
            checkForDownloads();
            return;
        }

        geode::utils::web::WebRequest req = geode::utils::web::WebRequest();

        req.userAgent("geometrydash-wakatime");

        try {
            webListener.setFilter(req.get(WAKATIME_VERSION_URL));
            webListener.bind([](geode::utils::web::WebTask::Event* event) {
                if (geode::utils::web::WebResponse* res = event->getValue()) {
                    matjson::Value object = res->json().unwrap();
                    s_latestVersion = object["tag_name"].asString().unwrapOr("").substr(1);
                    cacheLatestVersion();
                    checkForDownloads();
                } else if (event->isCancelled()) {
                    // no connection to github, aborting installation
                    s_shouldInstall = false;
                }
            });
        } catch (const std::exception& e) {
            s_shouldInstall = false;
            geode::log::error("Error fetching WakaTime version: {}", e);
        }
    }

    std::string getCachedVersion() {
        if (!geode::Mod::get()->hasSavedValue("savedAt") || !geode::Mod::get()->hasSavedValue("latestVersion")) return "";
        
        int savedAt = geode::Mod::get()->getSavedValue<int>("savedAt");

        int hours = std::chrono::duration_cast<std::chrono::hours>(std::chrono::system_clock::now().time_since_epoch()).count();

        // force a check for caches older than 3 hours
        if (hours - savedAt < 3) return geode::Mod::get()->getSavedValue<std::string>("latestVersion");

        return "";
    }

    void cacheLatestVersion() {
        geode::Mod::get()->setSavedValue<std::string>("latestVersion", s_latestVersion);

        int hours = std::chrono::duration_cast<std::chrono::hours>(std::chrono::system_clock::now().time_since_epoch()).count();

        geode::Mod::get()->setSavedValue<int>("savedAt", hours);
    }

    bool isLatest() {
        if (!isInstalled()) return false;
        if (!getGlobalPath().empty()) return true;

        std::string current = getCurrentVersion();

        // if there is no connection to github and wakatime-cli is downloaded, we leave it be
        if (!s_shouldInstall && !current.empty()) return true;

        return semver::equal(s_latestVersion, current);
    }

    std::string getDownloadURL() {
        std::string path = fmt::format("wakatime-cli-{}-{}.zip", os, arch);

        return WAKATIME_DOWNLOAD_URL + path;
    }

    void checkForDownloads() {
        if (isLatest()) return onChecksFinished();
        if (!s_shouldInstall) return onChecksFinished();

        std::string downloadURL = getDownloadURL();

        geode::utils::web::WebRequest req = geode::utils::web::WebRequest();

        req.userAgent("geometrydash-wakatime");

        try {
            downloadListener.setFilter(req.get(downloadURL));
            downloadListener.bind([](geode::utils::web::WebTask::Event* event) {
                if (geode::utils::web::WebResponse* res = event->getValue()) {
                    try {
                        geode::ByteVector data = res->data();
                        auto unzipResult = geode::utils::file::Unzip::create(data);
                        if (unzipResult) {
                            auto _ = unzipResult.unwrap().extractAllTo(s_resourcePath);
                            s_installed = true;
                            s_currentVersion = s_latestVersion;
                            geode::log::info("WakaTime CLI downloaded successfully");
                        } else {
                            geode::log::error("Failed to unzip downloaded data");
                        }
                    } catch (const std::exception& e) {
                        geode::log::error("Error processing downloaded WakaTime CLI: {}", e.what());
                    }

                    downloadListener.disable();
                    onChecksFinished();
                } else if (event->isCancelled()) {
                    geode::log::warn("WakaTime CLI download was cancelled");
                    downloadListener.disable();
                    onChecksFinished();
                }
            });
        } catch (const std::exception& e) {
            geode::log::error("Error downloading WakaTime CLI: {}", e);
            onChecksFinished();
        }
    }

    void onChecksFinished() {
        // wakatime not installed, don't start the heartbeats
        if (!isInstalled()) return geode::log::info("Unable to start WakaTime");
        
        geode::log::info("Downloading finalized, starting heartbeats");
        wakatime::startHeartbeats();
    }

    bool execute(const std::filesystem::path& path, const std::vector<std::string>& args) {
        std::string command = utils::quote(path.string());

        for (const auto& arg : args) {
            command += " " + arg;
        }

        geode::log::debug("Executing WakaTime CLI command: {}", command);

        std::thread([command]() {
            int result = std::system(utils::quote(command).c_str());
            if (result != 0) geode::log::error("WakaTime CLI command failed");
        }).detach();

        return true;
    }

    std::string getVersion() {
        return s_currentVersion;
    }
}