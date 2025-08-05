#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Scale : public CCNode {
    protected:
        std::vector<float> m_positions;
        std::vector<std::string> m_scaleData;
        float m_height;
        bool init(float height);
    
    public:
        static Scale* create(float height);
        void loadScale(std::vector<std::string> data) {
            m_scaleData = data;
            updateScale();
        };
        void updateScale();
};