#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Scale : public CCNode {
    protected:
        std::vector<float> m_positions;
        std::vector<float> m_scaleData;
        float m_maxScale;
        float m_height;
        bool init(float height);
    
    public:
        static Scale* create(float height);
        void loadScale(std::vector<float> data) {
            m_scaleData = data;
            m_maxScale = *std::max_element(data.begin(), data.end());
            updateScale();
        };
        void updateScale();
        float getMaxScale() {
            return m_maxScale;
        }
};