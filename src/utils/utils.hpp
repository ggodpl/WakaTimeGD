#pragma once
#include <string>
#include <ctime>
#include <chrono>
#include <fmt/format.h>
#include <Geode/utils/string.hpp>
#include "../wakatime/tracker.hpp"

namespace utils {
    inline std::string quote(const std::string& str) {
        if (geode::utils::string::contains(str, ' ')) return fmt::format("\"{}\"", str);
        return str;
    }

    inline std::string categoryToString(tracker::ActivityCategory category, bool cliFriendly) {
        switch (category) {
            case tracker::ActivityCategory::Advising: return "advising";
            case tracker::ActivityCategory::Browsing: return "browsing";
            case tracker::ActivityCategory::Building: return "building";
            case tracker::ActivityCategory::CodeReviewing: return "code reviewing";
            case tracker::ActivityCategory::Coding: return "coding";
            case tracker::ActivityCategory::Communicating: return "communicating";
            case tracker::ActivityCategory::Debugging: return "debugging";
            case tracker::ActivityCategory::Designing: return "designing";
            case tracker::ActivityCategory::Indexing: return "indexing";
            case tracker::ActivityCategory::Learning: return "learning";
            case tracker::ActivityCategory::ManualTesting: return cliFriendly ? "manual testing" : "playtesting";
            case tracker::ActivityCategory::Meeting: return "meeting";
            case tracker::ActivityCategory::Planning: return "planning";
            case tracker::ActivityCategory::Researching: return "researching";
            case tracker::ActivityCategory::RunningTests: return "running tests";
            case tracker::ActivityCategory::Supporting: return "supporting";
            case tracker::ActivityCategory::Translating: return "translating";
            case tracker::ActivityCategory::WritingDocs: return "writing docs";
            case tracker::ActivityCategory::WritingTests: return "writing tests";
            case tracker::ActivityCategory::None:
            default: return "";
        }
    }

    inline std::string plural(const std::string& str, int val) {
        return val == 1 ? str : str + "s";
    }

    inline int pickStep(float amount, float aim) {
        std::vector<int> STEPS = { 1, 2, 5, 10, 15, 30, 60, 120, 300, 600, 900, 1800, 3600, 7200, 10800 };
        float target = amount / aim;
        for (int i = 0; i < STEPS.size(); i++) {
            if (STEPS[i] >= target) return STEPS[i];
        }

        return STEPS[STEPS.size() - 1];
    }

    inline std::string formatPrecNoTrail(float f) {
        float f_abs = std::abs(f);
        float rem = f_abs - std::floor(f_abs);
        if (rem > 0) return fmt::format("{:.1f}", f);
        return fmt::format("{}", f);
    }

    inline float roundNearest(float n, float x) {
        return (std::floor((n + x - 1) / x)) * x;
    }

    inline std::vector<float> getDivisions(float amount, int aim = 10) {
        int step = pickStep(amount, aim);

        float start = step;
        float end = amount + step;

        int count = std::ceil(end / step) + 1;
        
        std::vector<float> divisions;
        for (int i = 0; i < count; i++) {
            divisions.push_back(start + i * step);
        }

        return divisions;
    }
};