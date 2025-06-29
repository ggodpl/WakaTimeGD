#pragma once

#include <string>
#include <regex>

namespace semver {
    struct SemVer {
        int major;
        int minor;
        int patch;
        std::string preRelease;
        std::string metadata;

        bool valid = true;

        bool operator==(const SemVer& other) const {
            return major == other.major && minor == other.minor && patch == other.patch;
        }
    };

    inline SemVer parse(const std::string& v) {
        SemVer result;
            
        try {
            size_t firstPart = v.find('.');
            if (firstPart == std::string::npos) {
                throw std::invalid_argument("Invalid SemVer");
            }

            size_t secondPart = v.find('.', firstPart + 1);
            if (secondPart == std::string::npos) {
                throw std::invalid_argument("Invalid SemVer");
            }

            result.major = std::stoi(v.substr(0, firstPart));
            result.minor = std::stoi(v.substr(firstPart + 1, secondPart - firstPart - 1));

            size_t metadataPos = v.find('+', secondPart);
            size_t preReleasePos = v.find('-', secondPart);
            size_t patchEnd;

            if (preReleasePos != std::string::npos && (metadataPos == std::string::npos || preReleasePos < metadataPos)) {
                patchEnd = preReleasePos;
            } else if (metadataPos != std::string::npos) {
                patchEnd = metadataPos;
            } else {
                patchEnd = v.length();
            }

            result.patch = std::stoi(v.substr(secondPart + 1, patchEnd - secondPart - 1));

            if (preReleasePos != std::string::npos) {
                size_t preReleaseEnd = (metadataPos != std::string::npos) ? metadataPos : v.length();
                result.preRelease = v.substr(preReleasePos + 1, preReleaseEnd - preReleasePos - 1);
            }

            if (metadataPos != std::string::npos) {
                result.metadata = v.substr(metadataPos + 1);
            }
        } catch (const std::exception& e) {
            result.valid = false;
        }

        return result;
    }

    inline bool equal(const std::string& v1, const std::string& v2) {
        SemVer s1 = parse(v1);
        SemVer s2 = parse(v2);

        if (!s1.valid || !s2.valid) return false;

        return s1 == s2;
    }
}