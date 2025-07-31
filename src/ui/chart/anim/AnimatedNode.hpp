#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class AnimatedNode : public CCNode {
    public:
        static AnimatedNode* create() {
            auto ret = new AnimatedNode();
            if (ret->init()) {
                ret->autorelease();
                return ret;
            }

            delete ret;
            return nullptr;
        }

        void setPositionEase(const CCPoint& point) {
            float duration = 0.25f;

            auto action = CCMoveTo::create(duration, point);
            auto easing = CCEaseIn::create(action, 2.f);

            this->runAction(easing);
        }
};