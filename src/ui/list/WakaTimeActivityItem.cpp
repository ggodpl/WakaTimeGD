#include <fmt/format.h>
#include "WakaTimeActivityItem.hpp"
#include "../../utils/utils.hpp"

bool WakaTimeActivityItem::init(const std::string& name, int total) {
    if (!CCNode::init()) return false;
    
    m_name = name;
    m_total = total;
    
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
    
    m_totalTimeLabel = CCLabelBMFont::create(time.c_str(), "goldFont.fnt");
    m_totalTimeLabel->setAnchorPoint({ 1.f, 0.5f });
    m_totalTimeLabel->setPosition({ getItemSize().width - 15.f, getItemSize().height / 2 });
    m_totalTimeLabel->setScale(0.35f);
    addChild(m_totalTimeLabel, 2);
    
    return true;
}

WakaTimeActivityItem* WakaTimeActivityItem::create(const std::string& projectName, int totalTime) {
    auto ret = new WakaTimeActivityItem();
    if (ret->init(projectName, totalTime)) {
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}