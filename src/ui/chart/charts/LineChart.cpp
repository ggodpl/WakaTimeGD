#include "LineChart.hpp"
#include "../node/LinesNode.hpp"

void LineChart::addDataset(Dataset dataset) {
    std::vector<float> rawData = dataset.data;

    float gap = 5.f;
    float maxHeight = m_datasets->getContentHeight();

    ccColor4F color = {
        dataset.color.r / 255.f,
        dataset.color.g / 255.f,
        dataset.color.b / 255.f,
        0.75f
    };
    
    std::vector<CCPoint> points;

    for (int i = 0; i < rawData.size(); i++) {
        auto child = m_labels->getChildByTag(i);
        if (!child) continue;
        float x = child->getPositionX() + child->getContentWidth() / 2 + gap;

        float data = rawData[i];
        
        float ratio = data / *this->m_maxScale;
        float height = ratio * maxHeight;

        points.push_back({ x, height });
    }

    std::vector<CCPoint> base;
    for (auto p : points) {
        base.push_back({ p.x, maxHeight });
    }
    
    std::vector<CCPoint> previousPoints = m_points[dataset.id];

    if (previousPoints.size() < rawData.size()) {
        previousPoints = base;
    }

    auto lines = m_datasets->getChildByID(fmt::format("datasets_line/{}"_spr, dataset.id));

    if (!lines) {
        lines = LinesNode::create(previousPoints, color);
        lines->setID(fmt::format("datasets_line/{}"_spr, dataset.id));
        lines->setAnchorPoint({ 0, 0 });
        lines->setPosition({ 0, 0 });

        m_datasets->addChild(lines);
    } else {
        static_cast<LinesNode*>(lines)->snapTo(previousPoints);
    }

    m_points[dataset.id] = points;

    static_cast<LinesNode*>(lines)->animateTo(points, 0.25f);
}

bool LineChart::init(const CCSize& size) { return BaseChart::init(size); } 

LineChart* LineChart::create(const CCSize& size) {
    auto ret = new LineChart();
    if (ret->init(size)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}