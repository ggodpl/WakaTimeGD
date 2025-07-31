#include <fmt/format.h>
#include "WakaTimeProjectNew.hpp"
#include "list/WakaTimeActivityItem.hpp"
#include "chart/node/DayLabel.hpp"
#include "chart/node/DatasetSelector.hpp"
#include "chart/charts/BarChart.hpp"
#include "chart/charts/LineChart.hpp"
#include "chart/charts/MixedChart.hpp"
#include "../wakatime/cli.hpp"
#include "../wakatime/wakatime.hpp"
#include "../utils/utils.hpp"

// todo: remove this too
#include <cstdlib>
#include <ctime>

bool WakaTimeProjectNew::setup(const std::string& name, int total, bool weekly) {
    // todo: and this
    srand(time(0));
    
    m_baseLayout = ColumnLayout::create()
        ->setGrowCrossAxis(true)
        ->setAutoGrowAxis(0.f)
        ->setAutoScale(false)
        ->setAxisReverse(true)
        ->setCrossAxisAlignment(AxisAlignment::Start)
        ->setCrossAxisLineAlignment(AxisAlignment::Start)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setGap(0.f);

    m_scrollLayer = ScrollLayer::create({ m_mainLayer->getContentWidth(), m_mainLayer->getContentHeight() - 20 });
    m_scrollLayer->setPosition({ 0, 0 });
    m_mainLayer->addChild(m_scrollLayer);

    auto title = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
    title->setScale(0.75f);
    
    auto totalTimeCounter = fmt::format("Total: {}", ::utils::formatClean(total));
    auto weeklyTimeCounter = fmt::format("This week: {}", ::utils::formatClean(weekly));

    auto totalTime = CCLabelBMFont::create(totalTimeCounter.c_str(), "bigFont.fnt");
    totalTime->setColor({ 177, 177, 177 });
    totalTime->setScale(0.5f);

    auto weeklyTime = CCLabelBMFont::create(weeklyTimeCounter.c_str(), "bigFont.fnt");
    weeklyTime->setColor({ 177, 177, 177 });
    weeklyTime->setScale(0.5f);

    auto textNode = CCNode::create();
    textNode->setAnchorPoint({ 0, 1 });
    textNode->setLayout(m_baseLayout);
    textNode->addChild(title);
    textNode->addChild(totalTime);
    textNode->addChild(weeklyTime);
    textNode->updateLayout();

    auto datasetSelector = DatasetSelector::create(m_mainLayer->getContentWidth() - 20, { "17 Jul - 24 Jul", "24 Jul - 31 Jul" });
    datasetSelector->setContentSize({ m_mainLayer->getContentWidth() - 20, 20 });
    datasetSelector->skipToLast();

    m_barChart = MixedChart::create({ m_mainLayer->getContentWidth() - 20, 155 });
    m_barChart->setAnchorPoint({ 0, 1 });
    m_barChart->loadScale(::utils::getDivisions(3 * 60 * 60 + 24 * 60));
    auto mixed = static_cast<MixedChart*>(m_barChart);
    mixed->setBarWidth(12.5f);
    mixed->setLineThickness(.5f);

    auto wrapperNode = CCNode::create();
    wrapperNode->setLayout(m_baseLayout->setGap(10.f));
    wrapperNode->setContentSize({ m_mainLayer->getContentWidth() - 20, 0.f });
    wrapperNode->setPosition({ 10, m_mainLayer->getContentHeight() - 20 });
    wrapperNode->setAnchorPoint({ 0, 1 });
    wrapperNode->addChild(textNode);
    wrapperNode->addChild(datasetSelector);
    wrapperNode->addChild(m_barChart);
    wrapperNode->updateLayout();

    m_scrollLayer->addChild(wrapperNode);

    auto debugSprite = CircleButtonSprite::createWithSprite("wakatimeDebugIcon.png"_spr, 0.95f, CircleBaseColor::Gray, CircleBaseSize::Small);
    debugSprite->setScale(0.7f);

    auto debugButton = CCMenuItemSpriteExtra::create(
        debugSprite,
        this,
        menu_selector(WakaTimeProjectNew::test)
    );
    debugButton->setPosition({ m_mainLayer->getContentWidth(), 0 });

    auto debugMenu = CCMenu::create();
    debugMenu->addChild(debugButton);
    debugMenu->setPosition({ 0, 0 });
    debugMenu->setAnchorPoint({0.5f, 0.5f});

    m_mainLayer->addChild(debugMenu);

    return true;
}

// todo: and this
void WakaTimeProjectNew::test(CCObject*) {
    m_barChart->cleanLabels();

    std::vector<float> dataset;
    std::vector<float> dataset2;

    for (int i = 0; i < 7; i++) {
        int random = rand() % (24 * 60 * 60);
        dataset.push_back(random);
        if (random == 0) {
            dataset2.push_back(0); 
            continue;
        }
        int random2 = rand() % (24 * 60 * 60);
        dataset2.push_back(random2);
    }

    auto max = std::max(*std::max_element(dataset.begin(), dataset.end()), *std::max_element(dataset2.begin(), dataset2.end()));

    auto scale = ::utils::getDivisions(max);

    m_barChart->loadScale(scale);
    int randomDay = rand() % 7;
    std::vector<std::string> pastWeek = ::utils::getWeek(randomDay);

    for (int i = 0; i < 7; i++) {
        std::string day = pastWeek[i];
        float data = dataset[i];

        auto dayLabel = DayLabel::create(day, data);
        dayLabel->setTag(i);

        m_barChart->addLabel(dayLabel);
    }

    auto mixedCast = static_cast<MixedChart*>(m_barChart);

    mixedCast->addBarDataset({ dataset, "total", "total", { 31, 184, 255 } });
    mixedCast->addLineDataset({ dataset2, "not total", "not-total", { 31, 255, 106 }, false });
}

void WakaTimeProjectNew::loadProject(const std::string& name, bool weekly) {
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

WakaTimeProjectNew* WakaTimeProjectNew::create(const std::string& name, int total, bool weekly) {
    auto ret = new WakaTimeProjectNew();
    if (ret->initAnchored(375.f, 280.f, name, total, weekly, "wakatimeMenuBackground.png"_spr)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}