#include <fmt/format.h>
#include "WakaTimeMenu.hpp"
#include "WakaTimeDebugMenu.hpp"
#include "list/WakaTimeProjectItem.hpp"
#include "../wakatime/cli.hpp"
#include "../wakatime/wakatime.hpp"
#include "../utils/time.hpp"

bool WakaTimeMenu::setup() {
    setTitle("Tracked projects");

    m_scrollLayer = ScrollLayer::create({ m_mainLayer->getContentWidth() - 20, m_mainLayer->getContentHeight() - 50 });
    m_scrollLayer->setPosition({ 10, 15 });
    m_mainLayer->addChild(m_scrollLayer);

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

    loadProjects();

    return true;
}

void WakaTimeMenu::loadProjects() {
    std::string valueKey = "localTime";

    if (!geode::Mod::get()->hasSavedValue(valueKey)) {
        auto noDataLabel = CCLabelBMFont::create("No tracked projects", "bigFont.fnt");
        noDataLabel->setPosition({ m_scrollLayer->getContentWidth() / 2, m_scrollLayer->getContentHeight() / 2 });
        noDataLabel->setScale(0.5f);
        m_scrollLayer->m_contentLayer->addChild(noDataLabel);
        return;
    }

    matjson::Value data = geode::Mod::get()->getSavedValue<matjson::Value>(valueKey);
    auto projects = data["projects"];

    if (!projects.isObject()) {
        auto noDataLabel = CCLabelBMFont::create("No tracked projects", "bigFont.fnt");
        noDataLabel->setPosition({ m_scrollLayer->getContentWidth() / 2, m_scrollLayer->getContentHeight() / 2 });
        noDataLabel->setScale(0.5f);
        m_scrollLayer->m_contentLayer->addChild(noDataLabel);
        return;
    }

    auto items = CCMenu::create();

    std::vector<std::tuple<std::string, int, int>> projectList;

    for (auto& [name, project] : projects) {
        int total = project["total"].asInt().unwrapOr(0);
        
        int weekly = 0;

        auto week = time_utils::getWeek(0);

        for (auto date : week.dates) {
            weekly += project["days"][date]["total"].asInt().unwrapOr(0);
        }

        projectList.push_back({ name, total, weekly });
    }

    float itemHeight = WakaTimeProjectItem::getItemSize().height;
    float spacer = 5.f;
    float ratio = (itemHeight + spacer) / itemHeight;
    float anchorY = 1.f - ratio * std::max(5, int(projectList.size()));

    int tag = 0;

    for (auto& [name, total, weekly] : projectList) {
        auto item = WakaTimeProjectItem::create(name, total, weekly);
        item->setPosition({ m_scrollLayer->getContentWidth() / 2, -tag * (itemHeight + spacer) });
        item->setTag(tag);
        item->setAnchorPoint({ 0.5f, anchorY });
        items->addChild(item);

        tag += 1;
    }

    float totalHeight = projectList.size() * (itemHeight + spacer) + spacer;

    items->setContentSize({ m_scrollLayer->getContentWidth(), totalHeight });
    items->setPosition({ 0, 0 });

    m_scrollLayer->m_contentLayer->addChild(items);
    m_scrollLayer->m_contentLayer->setContentSize({ m_scrollLayer->getContentWidth(), std::max(totalHeight, m_scrollLayer->getContentHeight()) });
    m_scrollLayer->moveToTop();
}

void WakaTimeMenu::onDebugPressed(CCObject*) {
    auto debugMenu = WakaTimeDebugMenu::create();
    if (debugMenu) debugMenu->show();
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