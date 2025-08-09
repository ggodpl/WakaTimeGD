#include <Geode/Geode.hpp>
#include "tracker.hpp"
#include "wakatime.hpp"
#include "../utils/utils.hpp"
#include "../utils/time.hpp"

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
        onActivityChange(); // send heartbeat
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
 
        update(project, category, duration);
    }

    void ActivityTracker::update(const std::string& projectName, ActivityCategory category, int duration) {
        std::string day = time_utils::formatDay();
        auto categoryName = utils::categoryToString(category, false);
    
        matjson::Value data;
        if (geode::Mod::get()->hasSavedValue("localTime")) data = geode::Mod::get()->getSavedValue<matjson::Value>("localTime");
        else {
            // base structure
            data = matjson::Value::object();
            data["version"] = 1;
            data["projects"] = matjson::Value::object();

            auto metadata = matjson::Value::object();
            metadata["first_tracked"] = day;
            metadata["last_updated"] = day;
            metadata["total_projects"] = 2;
            metadata["total_days"] = 1;
            data["metadata"] = metadata;
        }

        data["metadata"]["last_updated"] = day;

        if (!data["projects"].contains(projectName)) {
            data["projects"][projectName] = matjson::Value::object();
            data["projects"][projectName]["days"] = matjson::Value::object();
            data["projects"][projectName]["total"] = 0;
            data["projects"][projectName]["first_tracked"] = day;
        }

        auto& project = data["projects"][projectName];
        if (!project["days"].contains(day)) {
            project["days"][day] = matjson::Value::object();
            project["days"][day]["total"] = 0;
            project["days"][day]["activities"] = matjson::Value::object();
        }

        project["total"] = project["total"].asInt().unwrapOr(0) + duration;

        auto& dayData = project["days"][day];
        dayData["total"] = dayData["total"].asInt().unwrapOr(0) + duration;

        auto& activities = dayData["activities"];
        activities[categoryName] = activities[categoryName].asInt().unwrapOr(0) + duration;

        geode::Mod::get()->setSavedValue<matjson::Value>("localTime", data);
    }

    std::string ActivityTracker::getProject() const {
        return currentProject;
    }

    std::string ActivityTracker::getCategory() const {
        return utils::categoryToString(currentCategory, true);
    }
}