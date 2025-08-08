#include <fmt/format.h>
#include "WakaTimeProjectItem.hpp"
#include "../WakaTimeProject.hpp"
#include "../../utils/time.hpp"

bool WakaTimeProjectItem::init(const std::string& name, int total, int weekly) {
    if (!CCNode::init()) return false;
    
    m_name = name;
    m_total = total;
    m_weekly = weekly;
    
    this->setContentSize(getItemSize());
    this->setAnchorPoint({0.5f, 0.5f});

    // background 
    auto m_bg = CCScale9Sprite::create("square02b_small.png");
    m_bg->setContentSize({ getItemSize().width - 2, getItemSize().height - 2 });
    m_bg->setPosition(getContentSize() / 2);
    m_bg->setColor({ 45, 53, 60 });
    m_bg->setOpacity(100);
    this->addChild(m_bg, 1);

    // labels
    m_nameLabel = CCLabelBMFont::create(m_name.c_str(), "bigFont.fnt");
    m_nameLabel->setAnchorPoint({ 0.f, 0.5f });
    m_nameLabel->setPosition({ 15, getItemSize().height / 2 });
    m_nameLabel->setScale(0.4f);
    m_nameLabel->setColor({ 255, 255, 255 });

    if (m_nameLabel->getScaledContentSize().width > 180.f) {
        m_nameLabel->setScale(0.35f);
        if (m_nameLabel->getScaledContentSize().width > 180.f) {
            std::string truncated = m_name.substr(0, 25) + "...";
            m_nameLabel->setString(truncated.c_str());
        }
    }
    
    this->addChild(m_nameLabel, 2);
    
    std::string time = time_utils::format(m_total);
    
    m_timeLabel = CCLabelBMFont::create(time.c_str(), "goldFont.fnt");
    m_timeLabel->setAnchorPoint({ 1.f, 0.5f });
    m_timeLabel->setPosition({ getItemSize().width - 25.f, getItemSize().height / 2 });
    m_timeLabel->setScale(0.35f);
    this->addChild(m_timeLabel, 2);

    // chevron so people know they can press the thing
    auto chevron = CCSprite::create("right.png"_spr);
    float scale = 10.f / chevron->getContentHeight();
    chevron->setScale(scale);
    chevron->setColor({ 255, 255, 255 });
    chevron->setOpacity(255);

    auto chevronMenu = CCMenuItemSpriteExtra::create(
        chevron,
        this,
        menu_selector(WeekSelector::prev)
    );
    chevronMenu->setAnchorPoint({ 1.f, 0.5f });
    chevronMenu->setPosition({ getItemSize().width - 10.f, getItemSize().height / 2});

    this->addChild(chevronMenu);

    // button
    auto buttonSprite = CCScale9Sprite::create("square02b_001.png");
    buttonSprite->setContentSize(getItemSize());
    buttonSprite->setOpacity(0);
    
    auto button = CCMenuItemSpriteExtra::create(
        buttonSprite,
        this,
        menu_selector(WakaTimeProjectItem::onItemPressed)
    );
    
    m_buttonMenu = CCMenu::create();
    m_buttonMenu->addChild(button);
    m_buttonMenu->setPosition(getContentSize() / 2);
    this->addChild(m_buttonMenu, 3);
    
    return true;
}

void WakaTimeProjectItem::onItemPressed(CCObject* sender) {
    auto project = WakaTimeProject::create(m_name, m_total, m_weekly);
    if (project) project->show();
}

WakaTimeProjectItem* WakaTimeProjectItem::create(const std::string& projectName, int total, int weekly) {
    auto ret = new WakaTimeProjectItem();
    if (ret->init(projectName, total, weekly)) {
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}