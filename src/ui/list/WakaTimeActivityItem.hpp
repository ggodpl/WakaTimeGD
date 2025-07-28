#pragma once
#include <Geode/Geode.hpp>
#include <string>

using namespace geode::prelude;

class WakaTimeActivityItem : public CCNode {
    protected:
        std::string m_name;
        int m_total;
        CCLabelBMFont* m_nameLabel;
        CCLabelBMFont* m_totalTimeLabel;
        CCScale9Sprite* m_bg;
        
        bool init(const std::string& projectName, int totalTime);
        
    public:
        static WakaTimeActivityItem* create(const std::string& projectName, int totalTime);
        
        std::string getName() const { return m_name; }
        int getTime() const { return m_total; }
        
        static CCSize getItemSize() { return { 320.f, 40.f }; }
};