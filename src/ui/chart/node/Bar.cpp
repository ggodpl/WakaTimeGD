#include "Bar.hpp"
#include "../anim/BarResizeAction.hpp"

bool Bar::init(const CCSize& size, const BarData& data) {
    if (!CCNode::init()) return false;

    this->setContentSize(size);

    const char* borderPath = data.rounded ? "border.png"_spr : "borderSq.png"_spr;
    const char* backgroundPath = data.rounded ? "fill.png"_spr : "fillSq.png"_spr;

    m_border = CCScale9Sprite::create(borderPath);
    m_border->setCapInsets({ 2, 2, 15, 15 });
    m_border->setContentSize(size);
    m_border->setPosition({ 0, 0 });
    m_border->setAnchorPoint({ 0, 0 });
    m_border->setOpacity(100);
    m_border->setColor(data.color);

    m_background = CCScale9Sprite::create(backgroundPath);
    m_background->setCapInsets({ 2, 2, 15, 15 });
    m_background->setContentSize(size);
    m_background->setPosition({ 0, 0 });
    m_background->setAnchorPoint({ 0, 0 });
    m_background->setOpacity(65);
    m_background->setColor(data.color);

    this->addChild(m_border);
    this->addChild(m_background);

    return true;
}

void Bar::rescaleY(float height) {
    this->m_background->setContentHeight(height);
    this->m_border->setContentHeight(height);
}

void Bar::resizeY(float ratio, float maxHeight) {
    float duration = 0.25f;

    auto action1 = BarResizeAction::create(duration, ratio * maxHeight);
    auto action2 = BarResizeAction::create(duration, ratio * maxHeight);

    auto easing1 = CCEaseSineInOut::create(action1);
    auto easing2 = CCEaseSineInOut::create(action2);
    
    this->m_background->runAction(easing1);
    this->m_border->runAction(easing2);
}

Bar* Bar::create(const CCSize& size, const BarData& data) {
    auto ret = new Bar();
    if (ret->init(size, data)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}