#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class WakaTimeMenu : public Popup<> {
    protected:
        bool m_weekly = true;
        bool m_toggled = false;
        ScrollLayer* m_scrollLayer;

        bool setup() override;
        void loadProjects();
        
        void onDebugPressed(CCObject*);
    public:
        static WakaTimeMenu* create();
};