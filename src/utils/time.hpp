#pragma once
#include <string>
#include <ctime>
#include <chrono>
#include <fmt/format.h>
#include "utils.hpp"

namespace time_utils {
    inline std::string formatDayClean(const std::tm& time) {
        std::string months[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

        return fmt::format("{:02d} {}", time.tm_mday, months[time.tm_mon]);
    }

    struct WeekData {
        std::vector<std::string> dates;
        std::vector<std::string> days;
        std::tm start;
        std::tm end;

        std::string format() {
            return fmt::format("{} - {}", formatDayClean(start), formatDayClean(end));
        }
    };

    inline std::tm parse(std::string time) {
        std::tm tm{};
        int year;
        int mon;
        sscanf(time.c_str(), "%d-%d-%d", &year, &mon, &tm.tm_mday);
        tm.tm_year = year - 1900;
        tm.tm_mon = mon - 1;
        return tm;
    }

    inline int getWeekOffset(const std::tm& time) {
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm tm = time;
        auto epoch = std::mktime(&tm);

        auto diff = now - epoch;
        return static_cast<int>(diff / (7 * 24 * 60 * 60));
    }

    inline std::string getDayName(int day) {
        std::string days[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

        return days[day];
    }

    inline std::string formatDay() {
        auto timeT = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::tm* localTime = std::localtime(&timeT);

        return fmt::format("{:04d}-{:02d}-{:02d}", localTime->tm_year + 1900, localTime->tm_mon + 1, localTime->tm_mday);
    }

    inline std::string ago(const std::chrono::system_clock::time_point& tp) {
        auto now = std::chrono::system_clock::now();
        
        int duration = std::chrono::duration_cast<std::chrono::seconds>(now - tp).count();
        int seconds = duration % 60;
        int minutes = (duration / 60) % 60;
        int hours = duration / 60 / 60;

        if (hours < 1 && minutes < 1) return fmt::format("{} {} ago", seconds, ::utils::plural("second", seconds));
        if (hours < 1) return fmt::format("{} {} ago", minutes, ::utils::plural("minute", minutes));
        return fmt::format("{} {}, {} {} ago", hours, ::utils::plural("hour", hours), minutes, ::utils::plural("minute", minutes));
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

        if (time.empty()) time = "0m 0s";
        
        return time;
    }

    inline std::string formatShort(float duration) {
        if (duration < 60) return fmt::format("{}s", ::utils::formatPrecNoTrail(duration));
        else if (duration < 3600) return fmt::format("{}m", ::utils::formatPrecNoTrail(duration / 60));
        return fmt::format("{}h", ::utils::formatPrecNoTrail(duration / 3600));
    }

    inline std::string formatClean(int duration) {
        int seconds = duration % 60;
        int minutes = (duration / 60) % 60;
        int hours = duration / 60 / 60;

        std::string time = "";
        if (hours > 0) time += fmt::format("{} {}", hours, ::utils::plural("hour", hours));
        if (minutes > 0) time += fmt::format("{}{} {}", !time.empty() ? ", " : "", minutes, ::utils::plural("minute", minutes));
        if (seconds > 0) time += fmt::format("{}{} {}", !time.empty() ? ", " : "", seconds, ::utils::plural("second", seconds));
        
        return time.empty() ? "0 seconds" : time;
    }

    inline WeekData getWeek(int ago) {
        auto now = std::chrono::system_clock::now();
        auto targetEnd = now - std::chrono::hours(24 * 7 * ago);

        WeekData week;
        for (int i = 6; i >= 0; i--) {
            auto day = targetEnd - std::chrono::hours(24 * i);
            auto timeT = std::chrono::system_clock::to_time_t(day);
            auto localTime = *std::localtime(&timeT);

            std::string date = fmt::format("{:04d}-{:02d}-{:02d}", localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday);
            week.dates.push_back(date);

            week.days.push_back(getDayName(localTime.tm_wday));

            if (i == 6) week.start = localTime;
            if (i == 0) week.end = localTime;
        }

        return week;
    }
}