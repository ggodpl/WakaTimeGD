#pragma once
#include <Geode/Geode.hpp>
#include <string>

using namespace geode::prelude;

class WakaTimeProjectItem : public CCNode {
    protected:
        std::string m_name;
        int m_total;
        int m_weekly;
        CCLabelBMFont* m_nameLabel;
        CCLabelBMFont* m_timeLabel;
        CCScale9Sprite* m_bg;
        CCMenu* m_buttonMenu;
        
        bool init(const std::string& projectName, int total, int weekly);
        
    public:
        static WakaTimeProjectItem* create(const std::string& projectName, int total, int weekly);
        
        void onItemPressed(CCObject* sender);
        
        std::string getName() const { return m_name; }
        int getTime() const { return m_total; }
        
        static CCSize getItemSize() { return { 320.f, 40.f }; }
};