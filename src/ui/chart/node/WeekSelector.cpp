#include "WeekSelector.hpp"
#include "../../../utils/time.hpp"

bool WeekSelector::init(float width, int max) {
    if (!CCNode::init()) return false;

    m_offset = 0;
    m_max = max;

    float height = 20.f;

    m_menu = CCMenu::create();
    m_menu->setPosition({ 0, 0 });
    m_menu->setAnchorPoint({ 0, 0 });
    m_menu->setContentSize({ width, height });
    
    auto leftSprite = CCSprite::create("left.png"_spr);
    float scale = height / leftSprite->getContentHeight();
    leftSprite->setScale(scale);

    auto rightSprite = CCSprite::create("right.png"_spr);
    rightSprite->setScale(scale);

    m_prev = CCMenuItemSpriteExtra::create(
        leftSprite,
        this,
        menu_selector(WeekSelector::prev)
    );
    m_prev->setPosition({ 0, height / 2 });
    m_prev->setAnchorPoint({ 0, 0.5f });

    m_next = CCMenuItemSpriteExtra::create(
        rightSprite,
        this,
        menu_selector(WeekSelector::next)
    );
    m_next->setPosition({ width, height / 2 });
    m_next->setAnchorPoint({ 1.f, 0.5f });

    m_menu->addChild(m_prev);
    m_menu->addChild(m_next);

    this->addChild(m_menu);

    updateCurrent();

    this->setContentWidth(m_menu->getContentWidth());

    return true;
}

void WeekSelector::updateCurrent() {
    if (m_offset == m_max) {
        m_prev->m_animationEnabled = false;
        m_prev->setColor({ 150, 150, 150 });
    } else {
        m_prev->m_animationEnabled = true;
        m_prev->setColor({ 255, 255, 255 });
    }

    if (m_offset == 0) {
        m_next->m_animationEnabled = false;
        m_next->setColor({ 150, 150, 150 });
    } else {
        m_next->m_animationEnabled = true;
        m_next->setColor({ 255, 255, 255 });
    }

    // race cond prevention
    if (m_offset < 0) m_offset = 0;
    if (m_offset > m_max) m_offset = m_max;

    auto label = CCLabelBMFont::create(time_utils::getWeek(m_offset).format().c_str(), "bigFont.fnt");
    label->setScale(0.65f);
    label->setPosition({ this->getContentWidth() / 2, 0 });
    label->setAnchorPoint({ 0.5f, 0 });
    label->setTag(247);

    m_menu->removeChildByTag(247);

    m_menu->addChild(label);
}

WeekSelector* WeekSelector::create(float width, int max) {
    auto ret = new WeekSelector();
    if (ret->init(width, max)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}