#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class WakaTimeProject : public Popup<const std::string&, int, bool> {
    protected:
        bool m_weekly = true;
        bool m_toggled = false;
        ScrollLayer* m_scrollLayer;
        CCMenuItemToggler* m_weeklyButton;
        CCMenuItemToggler* m_allTimeButton;
        CCMenu* m_radioMenu;

        bool setup(const std::string& name, int total, bool weekly);
        void loadProject(const std::string& name, bool weekly);
    public:
        static WakaTimeProject* create(const std::string& name, int total, bool weekly);
};