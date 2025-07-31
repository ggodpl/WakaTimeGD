#pragma once

#include "../core/BaseChart.hpp"

class LineChart : public BaseChart {
    protected:
        float m_lineThickness = .5f;
        std::map<std::string, std::vector<CCPoint>> m_points;
        bool init(const CCSize& size);
    public:
        void addDataset(Dataset dataset) override;
        void setLineThickness(float thickness) {
            m_lineThickness = thickness;
        };

        static LineChart* create(const CCSize& size);
};