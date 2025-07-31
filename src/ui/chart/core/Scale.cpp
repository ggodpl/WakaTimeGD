#include "Scale.hpp"
#include "../anim/AnimatedNode.hpp"
#include "../../../utils/utils.hpp"

bool Scale::init(float height) {
    if (!CCNode::init()) return false;

    m_height = height;

    return true;
}

void Scale::updateScale() {
    this->removeAllChildren();

    m_positions.resize(m_scaleData.size(), 0.f);

    for (int i = 0; i < m_scaleData.size(); i++) {
        float lastY = m_positions.size() > i ? m_positions[i] : m_height;
        float y = std::round((m_height / m_scaleData.size()) * (i + 1));

        auto node = AnimatedNode::create();
        node->setPosition({ 0, lastY });
        node->setAnchorPoint({ 0.f, 0.5f });
        node->setTag(i);
        node->setPositionEase({ 0, y });

        m_positions[i] = y;

        auto scale = CCLabelBMFont::create(::utils::formatShort(m_scaleData[i]).c_str(), "bigFont.fnt");
        scale->setScale(0.25f);
        scale->setAnchorPoint({ 1.f, 0.5f });
        scale->setColor({ 177, 177, 177 });

        node->addChild(scale);
        this->addChild(node);
    }
}

Scale* Scale::create(float height) {
    auto ret = new Scale();
    if (ret->init(height)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}