#include "BaseChart.hpp"
#include "../anim/AnimatedNode.hpp"
#include "../../../utils/utils.hpp"

bool BaseChart::init(const CCSize& size) {
    if (!CCNode::init()) return false;

    this->setContentSize(size);

    float offsetX = 20.f;
    float gap = 5.f;
    float labelHeight = 30.f;
    float frameThickness = 3.75f;
    int baseDividers = 10;
    CCSize drawNodeSize = { size.width - offsetX, size.height - labelHeight };

    m_divider = Divider::create(baseDividers);
    m_divider->setPosition({ offsetX, labelHeight });
    m_divider->setAnchorPoint({ 0, 0 });
    m_divider->setContentSize(drawNodeSize);
    m_divider->drawDividers();

    this->addChild(m_divider);

    m_scale = Scale::create(drawNodeSize.height);
    m_scale->setContentSize({ 15.f, drawNodeSize.height });
    m_scale->setPosition({ 15.f, labelHeight });

    this->addChild(m_scale);

    m_labels = CCNode::create();
    m_labels->setPosition({ offsetX + gap, 0 });
    m_labels->setContentSize({ size.width - offsetX - gap * 2, labelHeight });
    m_labels->setLayout(RowLayout::create()
        ->setAutoScale(false)
        ->setGrowCrossAxis(false)
        ->setAxisAlignment(AxisAlignment::Even)
        ->setCrossAxisAlignment(AxisAlignment::Even)
    );
    m_labels->setAnchorPoint({ 0.f, 0.f });

    this->addChild(m_labels);

    m_datasets = CCNode::create();
    m_datasets->setContentSize({ size.width - offsetX, size.height - labelHeight });
    m_datasets->setAnchorPoint({ 0, 0 });
    m_datasets->setPosition({ offsetX, labelHeight });

    this->addChild(m_datasets);

    return true;
};

BaseChart* BaseChart::create(const CCSize& size) {
    auto ret = new BaseChart();
    if (ret->init(size)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}