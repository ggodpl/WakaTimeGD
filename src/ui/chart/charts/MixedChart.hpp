#pragma once

#include "BarChart.hpp"
#include "LineChart.hpp"

class MixedChart : public BaseChart {
    private:
        BarChart* m_barChart;
        LineChart* m_lineChart;
    protected:
        bool init(const CCSize& size) {
            if (!BaseChart::init(size)) return false;

            m_barChart = BarChart::create(size);
            m_lineChart = LineChart::create(size);

            if (m_barChart) {
                m_barChart->retain();
                m_barChart->move(this);
            }

            if (m_lineChart) {
                m_lineChart->retain();
                m_lineChart->move(this);
            }
            
            return true;
        }
    public:
        virtual ~MixedChart() {
            if (m_barChart) m_barChart->release();
            if (m_lineChart) m_lineChart->release();
        }

        void addDataset(Dataset dataset) {};
        void addBarDataset(Dataset dataset) {
            if (m_barChart) m_barChart->addDataset(dataset);
        }
        void addLineDataset(Dataset dataset) {
            if (m_lineChart) m_lineChart->addDataset(dataset);
        }

        void setBarWidth(float width) {
            if (m_barChart) m_barChart->setBarWidth(width);
        }
        void setLineThickness(float thickness) {
            if (m_lineChart) m_lineChart->setLineThickness(thickness);
        }

        static MixedChart* create(const CCSize& size) {
            auto ret = new MixedChart();
            if (ret->init(size)) {
                ret->autorelease();
                return ret;
            }

            delete ret;
            return nullptr;
        }
};