#pragma once
#include <Geode/Geode.hpp>
#include "chart/core/BaseChart.hpp"

using namespace geode::prelude;

class WakaTimeProjectNew : public Popup<const std::string&, int, bool> {
    protected:
        bool m_weekly = true;
        bool m_toggled = false;
        ScrollLayer* m_scrollLayer;
        CCMenuItemToggler* m_weeklyButton;
        CCMenuItemToggler* m_allTimeButton;
        CCMenu* m_radioMenu;
        AxisLayout* m_baseLayout;
        BaseChart* m_barChart;

        // todo: remove this after testing
        void test(CCObject*);

        bool setup(const std::string& name, int total, bool weekly);
        void loadProject(const std::string& name, bool weekly);
    public:
        static WakaTimeProjectNew* create(const std::string& name, int total, bool weekly);
};