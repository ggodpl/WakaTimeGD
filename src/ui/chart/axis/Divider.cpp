#include "Divider.hpp"

bool Divider::init(int count) {
    if (!CCNode::init()) return false;
    
    m_count = count;

    return true;
}

void Divider::drawDividers() {
    this->removeAllChildren();

    m_positions.resize(m_count, 0);

    for (int i = 0; i < m_count; i++) {
        float lastY = m_positions.size() > i ? m_positions[i] : this->getContentHeight();
        float y = std::round((this->getContentHeight() / m_count) * (i + 1));

        auto divider = DividerNode::create();
        divider->setContentSize({ this->getContentWidth(), 0.25f });
        divider->setPosition({ 0, lastY });
        divider->setAnchorPoint({ 0, 0 });
        divider->setTag(i);
        divider->setPositionEase({ 0, y });

        m_positions[i] = y;

        this->addChild(divider);

        divider->drawDivider();
    }
}

Divider* Divider::create(int count) {
    auto ret = new Divider();
    if (ret->init(count)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}