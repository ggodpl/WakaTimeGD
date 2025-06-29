#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class WakaTimeMenu : public Popup<> {
    protected:
        bool m_weekly = true;
        bool m_toggled = false;
        ScrollLayer* m_scrollLayer;
        CCMenuItemToggler* m_weeklyButton;
        CCMenuItemToggler* m_allTimeButton;
        CCMenu* m_radioMenu;

        bool setup() override;
        void loadProjects(bool weekly);
        
        void onTogglePressed(CCObject*);
        void onDebugPressed(CCObject*);
        void onWeeklyPressed(CCObject* sender);
        void onAllTimePressed(CCObject* sender);

        void updateButtons();
    public:
        static WakaTimeMenu* create();
};