#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class DatasetSelector : public CCNode {
    protected:
        int m_currentDataset;
        int m_prevDataset;
        std::vector<std::string> m_datasets;

        CCMenuItemSpriteExtra* m_prev;
        CCMenuItemSpriteExtra* m_next;
        CCMenu* m_menu;

        bool init(float width, std::vector<std::string> datasets);
    public:
        static DatasetSelector* create(float width, std::vector<std::string> datasets);
        void updateCurrent();

        void setCurrentDataset(int dataset) {
            m_currentDataset = dataset;
        }
        int getCurrentDataset() {
            return m_currentDataset;
        }

        void prev(CCObject*) {
            m_prevDataset = m_currentDataset;
            int dataset = m_currentDataset - 1;
            if (dataset < 0) dataset = 0;
            m_currentDataset = dataset;
            updateCurrent();
        }
        void next(CCObject*) {
            m_prevDataset = m_currentDataset;
            int dataset = m_currentDataset + 1;
            if (dataset > m_datasets.size()) dataset = m_datasets.size() - 1;
            m_currentDataset = dataset;
            updateCurrent();
        }
        void skipToLast() {
            m_prevDataset = m_currentDataset;
            m_currentDataset = m_datasets.size() - 1;
            updateCurrent();
        }
};