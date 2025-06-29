#include <Geode/Geode.hpp>
#include "tracker.hpp"
#include "wakatime.hpp"
#include "../utils/utils.hpp"

namespace tracker {
    void ActivityTracker::pause() {
        paused = true;
        geode::log::debug("Heartbeat paused");
    }

    void ActivityTracker::resume() {
        paused = false;
        geode::log::debug("Heartbeat resumed");
    }

    bool ActivityTracker::canHB() const {
        return !paused && currentCategory != ActivityCategory::None;
    }

    void ActivityTracker::resetActivity() {
        checkActivity();
        currentProject = "";
        currentCategory = ActivityCategory::None;
        active = false;
        geode::log::debug("Activity cleared");
    }

    void ActivityTracker::onActivityChange() {
        wakatime::sendHeartbeat();
    }
    
    void ActivityTracker::updateActivity(const std::string& project, ActivityCategory category) {
        checkActivity();
        currentProject = project;
        currentCategory = category;
        active = true;
        activityStarted = std::chrono::system_clock::now();
        geode::log::debug("New activity: {} {}", getCategory(), project);
    }

    void ActivityTracker::checkActivity() {
        if (active && currentCategory != ActivityCategory::None) {
            onActivityChange();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - activityStarted);
            logActivity(currentProject, currentCategory, duration.count());
        }
    }

    void ActivityTracker::logActivity(const std::string& project, ActivityCategory category, int duration) {
        if (duration < 1) return;

        // update both all-time and weekly 
        update(project, category, duration, false);
        update(project, category, duration, true);
    }

    void ActivityTracker::update(const std::string& project, ActivityCategory category, int duration, bool weekly) {
        std::string day = utils::formatDay();
        std::string week = utils::formatWeek();

        std::string valueKey = "timeTracking";
        if (weekly) valueKey += "_" + week;
    
        matjson::Value data;
        if (geode::Mod::get()->hasSavedValue(valueKey)) data = geode::Mod::get()->getSavedValue<matjson::Value>(valueKey);
        else data = matjson::Value::object();

        int weekTotal = data["total"].asInt().unwrapOr(0);
        data["total"] = weekTotal + duration;

        if (!data["projects"].isObject()) data["projects"] = matjson::Value::object();
        if (!data["projects"][project].isObject()) {
            data["projects"][project] = matjson::Value::object();
            data["projects"][project]["total"] = 0;
            data["projects"][project]["categories"] = matjson::Value::object();
        }

        int projectTotal = data["projects"][project]["total"].asInt().unwrapOr(0);
        data["projects"][project]["total"] = projectTotal + duration;

        std::string categoryName = utils::categoryToString(category, false);

        if (!data["projects"][project]["categories"].isObject()) data["projects"][project]["categories"] = matjson::Value::object();
        if (!data["projects"][project]["categories"][categoryName].isObject()) {
            data["projects"][project]["categories"][categoryName] = matjson::Value::object();
            data["projects"][project]["categories"][categoryName]["total"] = 0;
            data["projects"][project]["categories"][categoryName]["activity"] = matjson::Value::object();
        }

        int categoryTotal = data["projects"][project]["categories"][categoryName]["total"].asInt().unwrapOr(0);
        data["projects"][project]["categories"][categoryName]["total"] = categoryTotal + duration;

        if (!data["projects"][project]["categories"][categoryName]["activity"].isObject()) data["projects"][project]["categories"][categoryName]["activity"] = matjson::Value::object();
        
        int activityTotal = data["projects"][project]["categories"][categoryName]["activity"][day].asInt().unwrapOr(0);
        data["projects"][project]["categories"][categoryName]["activity"][day] = activityTotal + duration;

        geode::Mod::get()->setSavedValue(valueKey, data);
        auto _ = geode::Mod::get()->saveData();
    }

    std::string ActivityTracker::getProject() const {
        return currentProject;
    }

    std::string ActivityTracker::getCategory() const {
        return utils::categoryToString(currentCategory, true);
    }
}