#include <fmt/format.h>
#include "WakaTimeMenu.hpp"
#include "WakaTimeDebugMenu.hpp"
#include "list/WakaTimeProjectItem.hpp"
#include "../wakatime/cli.hpp"
#include "../wakatime/wakatime.hpp"
#include "../utils/utils.hpp"

bool WakaTimeMenu::setup() {
    setTitle("Weekly Projects");

    m_scrollLayer = ScrollLayer::create({ m_mainLayer->getContentWidth() - 20, m_mainLayer->getContentHeight() - 100 });
    m_scrollLayer->setPosition({ 10, 70 });
    m_mainLayer->addChild(m_scrollLayer);

    // RADIO MENU
    m_radioMenu = CCMenu::create();
    m_radioMenu->setPosition({ 0, 0 });
    m_radioMenu->setAnchorPoint({ 0.5f, 0.5f });
    m_mainLayer->addChild(m_radioMenu);

    auto weeklyNormal = ButtonSprite::create("Weekly", "bigFont.fnt", "GJ_button_04.png", 0.5f);
    auto weeklySelected = ButtonSprite::create("Weekly", "bigFont.fnt", "GJ_button_01.png", 0.5f);

    m_weeklyButton = CCMenuItemToggler::create(
        weeklySelected,
        weeklyNormal,
        this,
        menu_selector(WakaTimeMenu::onWeeklyPressed)
    );
    m_weeklyButton->setPosition({ m_mainLayer->getContentWidth() / 2 - m_weeklyButton->getContentWidth() / 2 - 5, 10 + m_weeklyButton->getContentHeight() / 2 });
    m_weeklyButton->setTag(0);
    m_weeklyButton->toggle(true);
    m_radioMenu->addChild(m_weeklyButton);

    auto allTimeNormal = ButtonSprite::create("All time", "bigFont.fnt", "GJ_button_04.png", 0.5f);
    auto allTimeSelected = ButtonSprite::create("All time", "bigFont.fnt", "GJ_button_01.png", 0.5f);

    m_allTimeButton = CCMenuItemToggler::create(
        allTimeSelected,
        allTimeNormal,
        this,
        menu_selector(WakaTimeMenu::onAllTimePressed)
    );
    m_allTimeButton->setPosition({ m_mainLayer->getContentWidth() / 2 + m_allTimeButton->getContentWidth() / 2 + 5, 10 + m_allTimeButton->getContentHeight() / 2 });
    m_allTimeButton->setTag(1);
    m_allTimeButton->toggle(false);
    m_radioMenu->addChild(m_allTimeButton);

    m_mainLayer->addChild(m_radioMenu);

    // DEBUG BUTTON
    auto debugSprite = CircleButtonSprite::createWithSprite("wakatimeDebugIcon.png"_spr, 0.95f, CircleBaseColor::Gray, CircleBaseSize::Small);
    debugSprite->setScale(0.7f);

    auto debugButton = CCMenuItemSpriteExtra::create(
        debugSprite,
        this,
        menu_selector(WakaTimeMenu::onDebugPressed)
    );
    debugButton->setPosition({ m_mainLayer->getContentWidth(), 0 });

    auto debugMenu = CCMenu::create();
    debugMenu->addChild(debugButton);
    debugMenu->setPosition({ 0, 0 });
    debugMenu->setAnchorPoint({0.5f, 0.5f});

    m_mainLayer->addChild(debugMenu);

    loadProjects(true);

    return true;
}

void WakaTimeMenu::loadProjects(bool weekly) {
    std::string week = ::utils::formatWeek();

    std::string valueKey = "timeTracking";
    if (weekly) valueKey += "_" + week;

    if (!geode::Mod::get()->hasSavedValue(valueKey)) {
        auto noDataLabel = CCLabelBMFont::create(fmt::format("No tracked projects{}", weekly ? " this week" : "").c_str(), "bigFont.fnt");
        noDataLabel->setPosition({ m_scrollLayer->getContentWidth() / 2, m_scrollLayer->getContentHeight() / 2 });
        noDataLabel->setScale(0.5f);
        m_scrollLayer->m_contentLayer->addChild(noDataLabel);
        return;
    }

    matjson::Value data = geode::Mod::get()->getSavedValue<matjson::Value>(valueKey);
    auto projects = data["projects"];

    if (!projects.isObject()) {
        auto noDataLabel = CCLabelBMFont::create(fmt::format("No tracked projects{}", weekly ? " this week" : "").c_str(), "bigFont.fnt");
        noDataLabel->setPosition({ m_scrollLayer->getContentWidth() / 2, m_scrollLayer->getContentHeight() / 2 });
        noDataLabel->setScale(0.5f);
        m_scrollLayer->m_contentLayer->addChild(noDataLabel);
        return;
    }

    auto items = CCMenu::create();
    float totalHeight = 0;
    float offsetY = 0;

    std::vector<std::pair<std::string, int>> projectList;

    for (auto& [name, project] : projects) {
        int total = project["total"].asInt().unwrapOr(0);
        projectList.push_back({name, total});
    }

    int tag = 0;

    for (auto& [name, total] : projectList) {
        auto item = WakaTimeProjectItem::create(name, total, weekly);
        item->setPosition({ m_scrollLayer->getContentWidth() / 2, offsetY });
        item->setTag(tag);
        items->addChild(item);

        int height = WakaTimeProjectItem::getItemSize().height + 5;

        offsetY -= height;
        totalHeight += height;
        tag += 1;
    }

    if (!projectList.empty()) totalHeight -= 5;

    items->setContentSize({ m_scrollLayer->getContentWidth(), totalHeight });
    items->setPosition({ 0, m_scrollLayer->getContentHeight() - totalHeight / 2 });

    m_scrollLayer->m_contentLayer->addChild(items);
    m_scrollLayer->m_contentLayer->setContentSize({ m_scrollLayer->getContentWidth(), std::max(totalHeight, m_scrollLayer->getContentHeight()) });
    m_scrollLayer->moveToTop();
}

void WakaTimeMenu::onTogglePressed(CCObject*) {
    m_weekly = !m_weekly;

    m_scrollLayer->m_contentLayer->removeAllChildren();
    
    if (m_weekly) setTitle("Weekly Projects");
    else setTitle("All Projects");
    
    loadProjects(m_weekly);
}

void WakaTimeMenu::onDebugPressed(CCObject*) {
    auto debugMenu = WakaTimeDebugMenu::create();
    if (debugMenu) debugMenu->show();
}

void WakaTimeMenu::updateButtons() {
    if (m_weekly) m_allTimeButton->toggle(false);
    else m_weeklyButton->toggle(false);
}

void WakaTimeMenu::onWeeklyPressed(CCObject*) {
    if (m_weekly) {
        m_weeklyButton->m_toggled = false;
        return;
    }

    m_weekly = true;
    updateButtons();

    m_scrollLayer->m_contentLayer->removeAllChildren();
    setTitle("Weekly Projects");
    loadProjects(m_weekly);
}

void WakaTimeMenu::onAllTimePressed(CCObject*) {
    if (!m_weekly) {
        m_allTimeButton->m_toggled = false;
        return;
    }

    m_weekly = false;
    updateButtons();

    m_scrollLayer->m_contentLayer->removeAllChildren();
    setTitle("All Time Projects");
    loadProjects(m_weekly);
}

WakaTimeMenu* WakaTimeMenu::create() {
    auto ret = new WakaTimeMenu();
    if (ret->initAnchored(350.f, 280.f, "wakatimeMenuBackground.png"_spr)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}