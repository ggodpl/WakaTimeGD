#include "DividerNode.hpp"

void DividerNode::drawDivider() {
    float width = this->getContentWidth();

    float segmentSize = 4.f;
    int lines = std::floor(width / segmentSize);

    for (int i = 0; i < lines; i++) {
        float startX = i * segmentSize;
        float endX = startX + 2.f;
        bool end = false;
        
        if (endX > width) {
            endX = width;
            end = true;
        }
        
        if (startX < width) {
            this->drawSegment({ startX, 0 }, { endX, 0 }, .25f, { 0.34, 0.34, 0.34, 0.75 });
        }

        if (end) break;
    }
}

void DividerNode::setPositionEase(const CCPoint& point) {
    float duration = 0.25f;

    auto action = CCMoveTo::create(duration, point);
    auto easing = CCEaseIn::create(action, 2.f);

    this->runAction(easing);
}

DividerNode* DividerNode::create() {
    auto ret = new DividerNode();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}