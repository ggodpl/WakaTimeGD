#include "DatasetSelector.hpp"

bool DatasetSelector::init(float width, std::vector<std::string> datasets) {
    if (!CCNode::init()) return false;

    m_currentDataset = 0;
    m_datasets = datasets;

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
        menu_selector(DatasetSelector::prev)
    );
    m_prev->setPosition({ 0, height / 2 });
    m_prev->setAnchorPoint({ 0, 0.5f });

    m_next = CCMenuItemSpriteExtra::create(
        rightSprite,
        this,
        menu_selector(DatasetSelector::next)
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

void DatasetSelector::updateCurrent() {
    if (m_currentDataset >= m_datasets.size() - 1) {
        m_next->m_animationEnabled = false;
        m_next->setColor({ 150, 150, 150 });
    } else {
        m_next->m_animationEnabled = true;
        m_next->setColor({ 255, 255, 255 });
    }

    if (m_currentDataset <= 0) {
        m_prev->m_animationEnabled = false;
        m_prev->setColor({ 150, 150, 150 });
    } else {
        m_prev->m_animationEnabled = true;
        m_prev->setColor({ 255, 255, 255 });
    }

    // race cond prevention
    if (m_currentDataset < 0) m_currentDataset = 0;
    if (m_currentDataset >= m_datasets.size()) m_currentDataset = m_datasets.size() - 1;

    auto label = CCLabelBMFont::create(m_datasets[m_currentDataset].c_str(), "bigFont.fnt");
    label->setScale(0.65f);
    label->setPosition({ this->getContentWidth() / 2, this->getContentHeight() / 2 });
    label->setAnchorPoint({ 0.5f, 0.5f });
    label->setTag(247);

    m_menu->removeChildByTag(247);

    m_menu->addChild(label);
}

DatasetSelector* DatasetSelector::create(float width, std::vector<std::string> datasets) {
    auto ret = new DatasetSelector();
    if (ret->init(width, datasets)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}