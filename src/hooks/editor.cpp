#include <Geode/Geode.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>
#include <Geode/modify/EditorPauseLayer.hpp>
#include "../wakatime/tracker.hpp"
#include "../wakatime/wakatime.hpp"

using namespace geode::prelude;

class $modify(LevelEditorLayerHook, LevelEditorLayer) {
    bool init(GJGameLevel* p0, bool p1) {
        if (!LevelEditorLayer::init(p0, p1)) return false;

        tracker::ActivityTracker::getInstance().updateActivity(p0->m_levelName, tracker::ActivityCategory::Designing);

        return true;
    };

    void onPlaytest() {
        tracker::ActivityTracker::getInstance().updateActivity(this->m_level->m_levelName, tracker::ActivityCategory::ManualTesting);

        LevelEditorLayer::onPlaytest();
    }
    
    void onStopPlaytest() {
        tracker::ActivityTracker::getInstance().updateActivity(this->m_level->m_levelName, tracker::ActivityCategory::Designing);

        LevelEditorLayer::onStopPlaytest();
    }
};

class $modify(EditorPauseLayerHook, EditorPauseLayer) {
    // if the pause menu is opened, pause the heartbeats
    bool init(LevelEditorLayer* p0) {
        if (!EditorPauseLayer::init(p0)) return false;

        tracker::ActivityTracker::getInstance().pause();

        return true;
    };

    void onResume(CCObject* sender) {
        tracker::ActivityTracker::getInstance().resume();

        EditorPauseLayer::onResume(sender);
    };

    // if the editor is closed, send and reset the activity
    void onExitEditor(CCObject* sender) {
        wakatime::sendHeartbeat();
        tracker::ActivityTracker::getInstance().resetActivity();

        EditorPauseLayer::onExitEditor(sender);
    };

    void onSaveAndPlay(CCObject* sender) {
        wakatime::sendHeartbeat();
        tracker::ActivityTracker::getInstance().resetActivity();

        EditorPauseLayer::onSaveAndPlay(sender);
    };
};