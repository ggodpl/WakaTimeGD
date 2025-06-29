#include <Geode/Geode.hpp>
#include <cstdlib>
#include <filesystem>
#include "tracker.hpp"
#include "wakatime.hpp"
#include "cli.hpp"
#include "../utils/utils.hpp"

namespace wakatime {
    std::thread heartbeatThread;
    std::atomic<bool> stopHeartbeat(false);

    std::atomic<std::chrono::system_clock::time_point> lastHeartbeat;

    static bool s_initialized = false;

    void init() {
        if (s_initialized) return;

        std::filesystem::path homeDirectory = getHomeDirectory();

        std::filesystem::path resourcePath = homeDirectory / ".wakatime";

        auto _ = geode::utils::file::createDirectory(resourcePath);

        cli::setResourcePath(resourcePath);
        cli::fetchLatestVersion();

        s_initialized = true;
    }

    void refetch() {
        geode::log::info("Refetching WakaTime after consent");
        cli::fetchLatestVersion();
    }

    void startHeartbeats() {
        stopHeartbeat = false;
        heartbeatThread = std::thread(wakatime::sendHeartbeatsWrapper);
    }

    void stopHeartbeats() {
        stopHeartbeat = true;
        if (heartbeatThread.joinable()) {
            heartbeatThread.join();
        }
    }

    void sendHeartbeatsWrapper() {
        while (!stopHeartbeat) {
            tracker::ActivityTracker tracker = tracker::ActivityTracker::getInstance();
            if (tracker.canHB()) wakatime::sendHeartbeat();
            lastHeartbeat = std::chrono::system_clock::now();

            std::this_thread::sleep_for(std::chrono::seconds(120));
        }
    }

    void sendHeartbeat() {
        if (!cli::isInstalled()) return;
        tracker::ActivityTracker tracker = tracker::ActivityTracker::getInstance();

        std::string project = tracker.getProject();
        std::string category = tracker.getCategory();

        if (category == "manual testing" && !geode::Mod::get()->getSettingValue<bool>("track-playtesting")) return;

        std::filesystem::path path = cli::getPath();

        #ifdef GEODE_GD_VERSION
            std::string gdVersion = "geometrydash/" + std::to_string(GEODE_GD_VERSION) + " ";
        #else
            std::string gdVersion = "";
        #endif

        std::vector<std::string> args = {
            "--plugin", utils::quote(gdVersion + "geometrydash-wakatime/1.0.0"),
            "--category", utils::quote(category),
            "--entity-type", "app",
            "--entity", "\"Geometry Dash\""
        };

        if (!project.empty()) {
            args.push_back("--project");
            args.push_back(utils::quote(project));
        }

        std::string apiKey = geode::Mod::get()->getSettingValue<std::string>("api-key");
        std::string apiURL = geode::Mod::get()->getSettingValue<std::string>("api-url");

        if (!apiKey.empty()) {
            args.push_back("--key");
            args.push_back(utils::quote(apiKey));
        }

        if (!apiURL.empty()) {
            args.push_back("--api-url");
            args.push_back(utils::quote(apiURL));
        }

        cli::execute(path, args);
    }

    std::filesystem::path getHomeDirectory() {
        const char* wakaHome = std::getenv("WAKATIME_HOME");

        if (wakaHome != nullptr && std::filesystem::exists(wakaHome)) {
            return wakaHome;
        }

        geode::log::info("No WAKATIME_HOME present, using home directory");

        const char* homeEnv =
            #ifdef GEODE_IS_WINDOWS
                std::getenv("USERPROFILE");
            #else
                std::getenv("HOME");
            #endif
            
        if (!homeEnv) {
            geode::log::error("Could not determine home directory, using CWD as fallback");
            return std::filesystem::current_path();
        }
        
        return std::filesystem::path(homeEnv);
    }

    std::chrono::system_clock::time_point getLastHeartbeat() {
        return lastHeartbeat;
    }
}