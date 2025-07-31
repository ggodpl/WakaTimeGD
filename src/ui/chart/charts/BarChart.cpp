#include "BarChart.hpp"
#include "../node/Bar.hpp"

void BarChart::addDataset(Dataset dataset) {
    std::vector<float> rawData = dataset.data;

    float gap = 5.f;
    
    m_datasets->removeChildByID(fmt::format("datasets_bar/{}"_spr, dataset.id));

    auto bars = CCNode::create();
    bars->setID(fmt::format("datasets_bar/{}"_spr, dataset.id));
    bars->setAnchorPoint({ 0, 0 });
    bars->setPosition({ 0, 0 });

    float maxHeight = m_datasets->getContentHeight();

    std::vector<float> barHeights = m_barHeights[dataset.id];

    if (barHeights.size() < rawData.size()) {
        barHeights.resize(rawData.size(), 0.f);
    }

    for (int i = 0; i < rawData.size(); i++) {
        auto child = m_labels->getChildByTag(i);
        if (!child) continue;
        float x = child->getPositionX() + child->getContentWidth() / 2 + gap;

        float data = rawData[i];
        float ratio = data / this->m_scale->getMaxScale();

        float height = barHeights.size() > i ? barHeights[i] : maxHeight;

        auto bar = Bar::create({ m_barWidth * 2, maxHeight }, { "test", dataset.name, dataset.color, dataset.rounded });
        bar->rescaleY(height);
        bar->setPosition({ x, 0.5f });
        bar->setAnchorPoint({ 0.5f, 0 });
        bar->setTag(i);
        bar->resizeY(ratio, maxHeight);

        barHeights[i] = ratio * maxHeight;
        
        bars->addChild(bar);
    }

    m_barHeights[dataset.id] = barHeights;

    m_datasets->addChild(bars);
}

bool BarChart::init(const CCSize& size) { return BaseChart::init(size); } 

BarChart* BarChart::create(const CCSize& size) {
    auto ret = new BarChart();
    if (ret->init(size)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}