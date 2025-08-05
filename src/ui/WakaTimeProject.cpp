#include <fmt/format.h>
#include "WakaTimeProject.hpp"
#include "chart/node/DayLabel.hpp"
#include "../wakatime/cli.hpp"
#include "../wakatime/wakatime.hpp"
#include "../utils/utils.hpp"
#include "../utils/time.hpp"

bool WakaTimeProject::setup(const std::string& name, int total, int weekly) {
    m_projectName = name;
    m_total = total;
    m_weekly = weekly;
    
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
    
    auto totalTimeCounter = fmt::format("Total: {}", time_utils::formatClean(total));
    auto weeklyTimeCounter = fmt::format("This week: {}", time_utils::formatClean(weekly));

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

    m_weekSelector = WeekSelector::create(m_mainLayer->getContentWidth() - 20, 0);
    m_weekSelector->setContentSize({ m_mainLayer->getContentWidth() - 20, 20 });
    m_weekSelector->bind([this]() { this->onWeekSelectorUpdate(); });
    m_weekSelector->updateCurrent();

    m_chart = MixedChart::create({ m_mainLayer->getContentWidth() - 20, 155 });
    m_chart->setAnchorPoint({ 0, 1 });
    auto divs = ::utils::getDivisions(3 * 60 * 60 + 24 * 60);
    std::vector<std::string> scale;
    for (auto div : divs) {
        scale.push_back(time_utils::formatShort(div));
    }
    m_chart->loadScale(scale, *std::max_element(divs.begin(), divs.end()));
    m_chart->setBarWidth(12.5f);
    m_chart->setLineThickness(.5f);

    auto wrapperNode = CCNode::create();
    wrapperNode->setLayout(m_baseLayout->setGap(10.f));
    wrapperNode->setContentSize({ m_mainLayer->getContentWidth() - 20, 0.f });
    wrapperNode->setPosition({ 10, m_mainLayer->getContentHeight() - 20 });
    wrapperNode->setAnchorPoint({ 0, 1 });
    wrapperNode->addChild(textNode);
    wrapperNode->addChild(m_weekSelector);
    wrapperNode->addChild(m_chart);
    wrapperNode->updateLayout();

    m_scrollLayer->m_contentLayer->addChild(wrapperNode);

    loadProject(name, 0);

    return true;
}

void WakaTimeProject::onWeekSelectorUpdate() {
    loadProject(m_projectName, m_weekSelector->getOffset());
}

bool WakaTimeProject::loadProject(const std::string& name, int offset) {
    m_chart->clearLabels();

    auto week = time_utils::getWeek(offset);

    if (!Mod::get()->hasSavedValue("localTime")) return false;

    auto data = Mod::get()->getSavedValue<matjson::Value>("localTime");
    if (!data.contains("projects") || !data["projects"].contains(name)) return false;

    auto& project = data["projects"][name];

    std::vector<float> totals;
    std::map<std::string, std::vector<float>> activities;

    float weeklyTotal = 0;
    float total = project["total"].as<float>().unwrapOr(0.f);

    for (int i = 0; i < week.dates.size(); i++) {
        const auto& date = week.dates[i];
        float total = project["days"][date]["total"].as<float>().unwrapOr(0.f);

        if (project["days"].contains(date)) {
            auto dayActivities = project["days"][date]["activities"];
            for (auto& [activity, duration] : dayActivities) {
                if (!activities.contains(activity)) {
                    activities[activity] = std::vector<float>(7, 0);
                }
                activities[activity][i] = duration.as<float>().unwrapOr(0.f);
            }
        }

        totals.push_back(total);
        weeklyTotal += total;

        auto label = DayLabel::create(week.days[i], total);
        label->setTag(i);

        m_chart->addLabel(label);
    }

    std::string firstTracked = project["first_tracked"].asString().unwrapOr(time_utils::formatDay());

    auto parsed = time_utils::parse(firstTracked);
    int weekOffset = time_utils::getWeekOffset(parsed);
    m_weekSelector->setMax(weekOffset);

    float max = *std::max_element(totals.begin(), totals.end());
    auto divs = ::utils::getDivisions(max);
    std::vector<std::string> scale;
    for (auto div : divs) {
        scale.push_back(time_utils::formatShort(div));
    }

    m_chart->loadScale(scale, *std::max_element(divs.begin(), divs.end()));

    // because of current bar chart limitations, if i want to stack several datasets i need cumulative data
    // cba to do this now
    // todo: do that within the bar chart module later
    std::map<std::string, std::vector<float>> cumData;
    std::vector<std::string> keys;

    for (auto& [key, _] : activities) {
        keys.push_back(key);
    }

    for (int i = 0; i < keys.size(); i++) {
        const auto& key = keys[i];
        int size = activities[key].size();
        cumData[key] = std::vector<float>(size, 0);

        for (int j = 0; j < size; j++) {
            float sum = 0;

            for (int l = i; l < keys.size(); l++) {
                sum += activities[keys[l]][j];
            }

            cumData[key][j] = sum;
        }
    }

    // this is also really stupid, so
    // todo: this should also be done automatically in stacked bar charts
    bool rounded = true;
    for (auto& [key, dataset] : cumData) {
        const auto& setting = fmt::format("color-{}", key);

        ccColor3B color;
        if (Mod::get()->hasSetting(setting)) color = Mod::get()->getSettingValue<ccColor3B>(setting);
        else color = { 31, 184, 255 };

        m_chart->addBarDataset({ dataset, key, key, color, rounded });
        rounded = false;
    }

    return true;
}

WakaTimeProject* WakaTimeProject::create(const std::string& name, int total, int weekly) {
    auto ret = new WakaTimeProject();
    if (ret->initAnchored(375.f, 280.f, name, total, weekly, "wakatimeMenuBackground.png"_spr)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}