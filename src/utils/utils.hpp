#pragma once
#include <string>
#include <ctime>
#include <chrono>
#include <fmt/format.h>
#include <Geode/utils/string.hpp>
#include "../wakatime/tracker.hpp"

namespace utils {
    inline std::string formatDay() {
        auto timeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm* localTime = std::localtime(&timeT);

        return fmt::format("{:04d}-{:02d}-{:02d}", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday);
    }

    inline std::string formatWeek(int offset = 0) {
        auto timeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm* localTime = std::localtime(&timeT);

        int year = localTime->tm_year + 1900;
        int day = localTime->tm_yday + 1 + (offset * 7);
        int weekDay = (localTime->tm_wday == 0) ? 7 : localTime->tm_wday;

        int week = (day - weekDay + 10) / 7;

        if (week == 0) {
            year--;
            week = 52;
        } else if (week > 52) {
            year++;
            week = 1;
        }

        return fmt::format("{}-W{:02d}", year, week);
    }

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

    inline std::string ago(const std::chrono::system_clock::time_point& tp) {
        auto now = std::chrono::system_clock::now();
        
        int duration = std::chrono::duration_cast<std::chrono::seconds>(now - tp).count();
        int seconds = duration % 60;
        int minutes = (duration / 60) % 60;
        int hours = duration / 60 / 60;

        if (hours < 1 && minutes < 1) return fmt::format("{} {} ago", seconds, plural("second", seconds));
        if (hours < 1) return fmt::format("{} {} ago", minutes, plural("minute", minutes));
        return fmt::format("{} {}, {} {} ago", hours, plural("hour", hours), minutes, plural("minute", minutes));
    }

    inline std::string format(int duration, int maxPrecision = 3) {
        int seconds = duration % 60;
        int minutes = (duration / 60) % 60;
        int hours = duration / 60 / 60;

        int precision = 0;

        std::string time = "";
        if (hours > 0 && precision < maxPrecision) {
            time += fmt::format("{}h", hours);
            precision++;
        }
        if (minutes > 0 && precision < maxPrecision) {
            time += fmt::format("{}{}m", !time.empty() ? " " : "", minutes);
            precision++;
        }
        if (seconds > 0 && precision < maxPrecision) time += fmt::format("{}{}s", !time.empty() ? " " : "", seconds);
        
        return time;
    }

    inline int pickStep(float duration, float aim) {
        std::vector<int> STEPS = { 1, 2, 5, 10, 15, 30, 60, 120, 300, 600, 900, 1800, 3600, 7200, 10800 };
        float target = duration / aim;
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

    inline std::string formatShort(float duration) {
        if (duration < 60) return fmt::format("{}s", formatPrecNoTrail(duration));
        else if (duration < 3600) return fmt::format("{}m", formatPrecNoTrail(duration / 60));
        return fmt::format("{}h", formatPrecNoTrail(duration / 3600));
    }

    inline std::string formatClean(int duration) {
        int seconds = duration % 60;
        int minutes = (duration / 60) % 60;
        int hours = duration / 60 / 60;

        std::string time = "";
        if (hours > 0) time += fmt::format("{} {}", hours, plural("hour", hours));
        if (minutes > 0) time += fmt::format("{}{} {}", !time.empty() ? ", " : "", minutes, plural("minute", minutes));
        if (seconds > 0) time += fmt::format("{}{} {}", !time.empty() ? ", " : "", seconds, plural("second", seconds));
        
        return time.empty() ? "0 seconds" : time;
    }

    inline float roundNearest(float n, float x) {
        return (std::floor((n + x - 1) / x)) * x;
    }

    inline std::vector<float> getDivisions(float duration, int aim = 10) {
        int step = pickStep(duration, aim);

        float start = step;
        float end = duration + step;

        int count = std::ceil(end / step) + 1;
        
        std::vector<float> divisions;
        for (int i = 0; i < count; i++) {
            divisions.push_back(start + i * step);
        }

        return divisions;
    }

    inline std::vector<std::string> getWeek(int day) {
        std::vector<std::string> days = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
        auto res = std::vector<std::string>(days.begin() + day + 1, days.end());
        res.insert(res.end(), days.begin(), days.end() - (7 - day - 1));
        return res;
    }
};