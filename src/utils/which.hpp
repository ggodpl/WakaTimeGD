#pragma once

#include <string>
#include <cstdlib>
#include <sstream>
#include <filesystem>
#include <Geode/utils/general.hpp>

#ifdef GEODE_IS_WINDOWS
#include <io.h>
#define ACCESS _access
#define X_OK 4
const char SEPARATOR = ';';
#else
#include <unistd.h>
#define ACCESS access
const char SEPARATOR = ':';
#endif

namespace which {
    inline bool is_exec(const std::filesystem::path& path) {
        std::error_code ec;

        if (!std::filesystem::exists(path, ec)) return false;
        if (!std::filesystem::is_regular_file(path, ec)) return false;

        return ACCESS(path.string().c_str(), X_OK) == 0;
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