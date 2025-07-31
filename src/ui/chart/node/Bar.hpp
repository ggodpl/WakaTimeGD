#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct BarData {
    const std::string& formattedData;
    const std::string& datasetName;
    const ccColor3B& color;
    bool rounded = true;
};

class Bar : public CCNode {
    protected:
        CCScale9Sprite* m_border;
        CCScale9Sprite* m_background;
        bool init(const CCSize& size, const BarData& data);
    
    public:
        static Bar* create(const CCSize& size, const BarData& data);

        void rescaleY(float ratio);
        void resizeY(float ratio, float maxHeight);
};