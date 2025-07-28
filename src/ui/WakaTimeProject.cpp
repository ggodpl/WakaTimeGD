#include <fmt/format.h>
#include "WakaTimeProject.hpp"
#include "list/WakaTimeActivityItem.hpp"
#include "../wakatime/cli.hpp"
#include "../wakatime/wakatime.hpp"
#include "../utils/utils.hpp"

bool WakaTimeProject::setup(const std::string& name, int total, bool weekly) {
    setTitle(name.c_str());

    m_scrollLayer = ScrollLayer::create({ m_mainLayer->getContentWidth() - 20, m_mainLayer->getContentHeight() - 145 });
    m_scrollLayer->setPosition({ 10, 70 });
    m_mainLayer->addChild(m_scrollLayer);

    std::string time = fmt::format("Total: {}", ::utils::format(total));

    auto totalTimeLabel = CCLabelBMFont::create(time.c_str(), "goldFont.fnt");
    totalTimeLabel->setPosition({ m_mainLayer->getContentWidth() / 2, m_mainLayer->getContentHeight() - 45 });
    totalTimeLabel->setScale(0.75f);
    m_mainLayer->addChild(totalTimeLabel);

    loadProject(name, weekly);

    return true;
}

void WakaTimeProject::loadProject(const std::string& name, bool weekly) {
    std::string week = ::utils::formatWeek();

    std::string valueKey = "timeTracking";
    if (weekly) valueKey += "_" + week;

    if (!geode::Mod::get()->hasSavedValue(valueKey)) {
        auto noDataLabel = CCLabelBMFont::create(fmt::format("No time tracked for {}{}", name, weekly ? " this week" : "").c_str(), "bigFont.fnt");
        noDataLabel->setPosition({ m_scrollLayer->getContentWidth() / 2, m_scrollLayer->getContentWidth() / 2 });
        noDataLabel->setScale(0.5f);
        m_scrollLayer->m_contentLayer->addChild(noDataLabel);
        return;
    }

    matjson::Value data = geode::Mod::get()->getSavedValue<matjson::Value>(valueKey);
    auto projects = data["projects"];

    if (!projects.isObject() || !projects[name].isObject() || !projects[name]["categories"].isObject()) {
        auto noDataLabel = CCLabelBMFont::create(fmt::format("No time tracked for {}{}", name, weekly ? " this week" : "").c_str(), "bigFont.fnt");
        noDataLabel->setPosition({ m_scrollLayer->getContentWidth() / 2, m_scrollLayer->getContentWidth() / 2 });
        noDataLabel->setScale(0.5f);
        m_scrollLayer->m_contentLayer->addChild(noDataLabel);
        return;
    }

    auto categories = projects[name]["categories"];

    auto items = CCMenu::create();

    std::vector<std::pair<std::string, int>> activities;

    for (auto& [name, activity] : categories) {
        int total = categories[name]["total"].asInt().unwrapOr(0);
        activities.push_back({name, total});
    }

    float totalHeight = (WakaTimeActivityItem::getItemSize().height + 5) * activities.size();
    if (!activities.empty()) totalHeight -= 5;

    float itemHeight = WakaTimeActivityItem::getItemSize().height;
    float spacer = 5.f;
    float ratio = (itemHeight + spacer) / itemHeight;
    float anchorY = std::min(-(ratio * activities.size() - 1.f), -(ratio * 3 - 1.f));

    int tag = 0;
    
    for (auto& [name, total] : activities) {
        auto item = WakaTimeActivityItem::create(name, total);
        item->setPosition({ m_scrollLayer->getContentWidth() / 2, -tag * (itemHeight + spacer) });
        item->setTag(tag);
        item->setAnchorPoint({ 0.5f, anchorY });
        items->addChild(item);

        tag += 1;
    }

    if (!activities.empty()) totalHeight -= 5;

    items->setContentSize({ m_scrollLayer->getContentWidth(), totalHeight });
    items->setPosition({ 0, 0 });

    m_scrollLayer->m_contentLayer->addChild(items);
    m_scrollLayer->m_contentLayer->setContentSize({ m_scrollLayer->getContentWidth(), std::max(totalHeight, m_scrollLayer->getContentHeight()) });
    m_scrollLayer->moveToTop();
}

WakaTimeProject* WakaTimeProject::create(const std::string& name, int total, bool weekly) {
    auto ret = new WakaTimeProject();
    if (ret->initAnchored(350.f, 280.f, name, total, weekly, "wakatimeMenuBackground.png"_spr)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}