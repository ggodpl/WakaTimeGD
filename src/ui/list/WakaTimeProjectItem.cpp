#include <fmt/format.h>
#include "WakaTimeProjectItem.hpp"
#include "../WakaTimeProject.hpp"
#include "../../utils/utils.hpp"

bool WakaTimeProjectItem::init(const std::string& name, int total, bool weekly) {
    if (!CCNode::init()) return false;
    
    m_name = name;
    m_total = total;
    m_weekly = weekly;
    
    setContentSize(getItemSize());
    setAnchorPoint({0.5f, 0.5f});

    // BACKGROUND
    
    auto m_bg = CCScale9Sprite::create("square02b_small.png");
    m_bg->setContentSize({ getItemSize().width - 2, getItemSize().height - 2 });
    m_bg->setPosition(getContentSize() / 2);
    m_bg->setColor({ 45, 53, 60 });
    m_bg->setOpacity(100);
    addChild(m_bg, 1);

    // LABELS
    
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
    
    addChild(m_nameLabel, 2);
    
    std::string time = ::utils::format(m_total);
    
    m_timeLabel = CCLabelBMFont::create(time.c_str(), "goldFont.fnt");
    m_timeLabel->setAnchorPoint({ 1.f, 0.5f });
    m_timeLabel->setPosition({ getItemSize().width - 15.f, getItemSize().height / 2 });
    m_timeLabel->setScale(0.35f);
    addChild(m_timeLabel, 2);

    // BUTTON
    
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
    addChild(m_buttonMenu, 3);
    
    return true;
}

void WakaTimeProjectItem::onItemPressed(CCObject* sender) {
    auto project = WakaTimeProject::create(m_name, m_total, m_weekly);
    if (project) project->show();
}

WakaTimeProjectItem* WakaTimeProjectItem::create(const std::string& projectName, int totalTime, bool weekly) {
    auto ret = new WakaTimeProjectItem();
    if (ret->init(projectName, totalTime, weekly)) {
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}