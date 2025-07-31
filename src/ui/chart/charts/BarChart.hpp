#pragma once

#include "../core/BaseChart.hpp"

class BarChart : public BaseChart {
    protected:
        float m_barWidth = 12.5f;
        std::map<std::string, std::vector<float>> m_barHeights;
        bool init(const CCSize& size);

    public:
        void addDataset(Dataset dataset) override;
        void setBarWidth(float width) {
            m_barWidth = width;
        };

        static BarChart* create(const CCSize& size);
};