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

    #ifdef GEODE_IS_MACOS
    void keyBackClicked() {
        tracker::ActivityTracker::getInstance().resume();

        EditorPauseLayer::keyBackClicked();
    }

    void FLAlert_Clicked(FLAlertLayer* p0, bool p1) {
        if (p0->getTag() == 1 && p1) {
            wakatime::sendHeartbeat();
            tracker::ActivityTracker::getInstance().resetActivity();
        }

        EditorPauseLayer::FLAlert_Clicked(p0, p1);
    }

    void onSaveAndExit(CCObject* sender) {
        if (!m_saved) {
            wakatime::sendHeartbeat();
            tracker::ActivityTracker::getInstance().resetActivity();
        }

        EditorPauseLayer::onSaveAndExit(sender);
    }

    void onExitNoSave(CCObject* sender) {
        if (!m_saved && m_editorLayer->m_level->m_levelType != GJLevelType::Editor) {
            wakatime::sendHeartbeat();
            tracker::ActivityTracker::getInstance().resetActivity();
        }

        EditorPauseLayer::onExitNoSave(sender);
    }
    #endif

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

#ifdef GEODE_IS_MACOS
static_assert(GEODE_COMP_GD_VERSION == 22074, "Incompatible GD version");

uintptr_t keyBackClickedAddress = base::get() + GEODE_ARM_MAC(0x22c97c) GEODE_INTEL_MAC(0x2830d0);

void hookKeyBackClicked(void* self) {
    wakatime::sendHeartbeat();
    tracker::ActivityTracker::getInstance().resetActivity();

    reinterpret_cast<void(*)(void*)>(keyBackClickedAddress)(self);
}

$execute {
    (void)Mod::get()->hook(
        reinterpret_cast<void*>(keyBackClickedAddress),
        &hookKeyBackClicked,
        "EditorPauseLayer::CCKeypadDelegate::keyBackClicked",
        tulip::hook::TulipConvention::Default
    );
}
#endif