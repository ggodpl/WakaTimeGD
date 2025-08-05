#pragma once
#include <Geode/Geode.hpp>
#include "chart/charts/MixedChart.hpp"
#include "chart/node/WeekSelector.hpp"

using namespace geode::prelude;

class WakaTimeProject : public Popup<const std::string&, int, int> {
    protected:
        int m_weekly;
        int m_total;

        bool m_toggled = false;
        ScrollLayer* m_scrollLayer;
        AxisLayout* m_baseLayout;
        MixedChart* m_chart;
        WeekSelector* m_weekSelector;

        std::string m_projectName;

        bool setup(const std::string& name, int total, int weekly);
        bool loadProject(const std::string& name, int offset = 0);
    public:
        static WakaTimeProject* create(const std::string& name, int total, int weekly);

        void onWeekSelectorUpdate();
};