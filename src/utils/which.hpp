#pragma once

#include <string>
#include <cstdlib>
#include <sstream>
#include <filesystem>
#include <Geode/utils/general.hpp>

#ifdef GEODE_IS_WINDOWS
const char SEPARATOR = ';';
#else
const char SEPARATOR = ':';
#endif

namespace which {
    inline bool is_exec(const std::filesystem::path& path) {
        std::error_code ec;

        if (!std::filesystem::exists(path, ec)) return false;
        if (!std::filesystem::is_regular_file(path, ec)) return false;

        #ifdef GEODE_IS_WINDOWS
            // i THINK windows files are generally executable anyway,
            // so if they exist and are regular that's PROBABLY good enough
            return true;
        #else
            auto perms = std::filesystem::status(path, ec).permissions();
            if (ec) return false;

            using std::filesystem::perms;
            return (perms & (perms::owner_exec | perms::group_exec | perms::others_exec)) != perms::none;
        #endif
    }

    inline std::string which(const std::string& exec) {
        std::string pathEnv = geode::utils::getEnvironmentVariable("PATH");
        if (pathEnv.empty()) return "";

        std::stringstream ss(pathEnv);
        std::string dir;

        while (std::getline(ss, dir, SEPARATOR)) {
            std::filesystem::path fullPath = std::filesystem::path(dir) / exec;

            #ifdef GEODE_IS_WINDOWS
                if (!fullPath.has_extension()) {
                    fullPath += ".exe";
                }
            #endif

            if (is_exec(fullPath)) {
                return fullPath.string();
            }
        }

        return "";
    }
}