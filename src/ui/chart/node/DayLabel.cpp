#include "DayLabel.hpp"
#include "../../../utils/utils.hpp"

bool DayLabel::init(const std::string& day, float duration) {
    if (!CCNode::init()) return true;

    this->setLayout(ColumnLayout::create()
        ->setAutoScale(false)
        ->setAutoGrowAxis(true)
        ->setAxisAlignment(AxisAlignment::Start)
        ->setCrossAxisAlignment(AxisAlignment::Center)
        ->setGap(0)
        ->setAxisReverse(true)
    );

    auto dayLabel = CCLabelBMFont::create(day.c_str(), "bigFont.fnt");
    dayLabel->setScale(0.3f);
    dayLabel->setColor({ 177, 177, 177 });

    auto timeLabel = CCLabelBMFont::create(::utils::format(duration, 2).c_str(), "bigFont.fnt");
    timeLabel->setScale(0.3f);
    timeLabel->setColor({ 255, 255, 255 });

    this->addChild(dayLabel);
    this->addChild(timeLabel);

    this->updateLayout();

    return true;
}

DayLabel* DayLabel::create(const std::string& day, float duration) {
    auto ret = new DayLabel();
    if (ret->init(day, duration)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}