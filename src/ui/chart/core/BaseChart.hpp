#pragma once
#include <Geode/Geode.hpp>
#include "Scale.hpp"
#include "../axis/Divider.hpp"

using namespace geode::prelude;

struct Dataset {
    std::vector<float> data;
    std::string name;
    std::string id;
    const ccColor3B& color;
    bool rounded = true; // i hate that it's in Dataset and not in something more BarChart specific
};

class BaseChart : public CCNode {
    protected:
        Scale* m_scale;
        CCNode* m_labels;
        Divider* m_divider;
        CCNode* m_datasets;
        std::shared_ptr<float> m_maxScale;
        
        bool init(const CCSize& size);
    public:
        BaseChart() : m_maxScale(std::make_shared<float>(0.f)) {};

        static BaseChart* create(const CCSize& size);
        
        void loadScale(std::vector<std::string> data, float maxValue) {
            m_scale->loadScale(data);
            m_divider->setCount(data.size());
            m_divider->drawDividers();
            *m_maxScale = maxValue;
        }
        void addLabel(CCNode* node) {
            m_labels->addChild(node);
            m_labels->updateLayout();
        }
        void clearLabels() {
            if (m_labels && m_labels->getChildrenCount() > 0) m_labels->removeAllChildren();
        }

        virtual void addDataset(Dataset dataset) {};

        void move(BaseChart* other) {
            m_datasets = other->m_datasets;
            m_scale = other->m_scale;
            m_labels = other->m_labels;
            m_divider = other->m_divider;
            m_maxScale = other->m_maxScale;
        }
};