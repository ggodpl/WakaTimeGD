#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BarResizeAction : public CCActionInterval {
    protected:
        float m_targetHeight;
        float m_startHeight;
        float m_deltaHeight;
    public:
        static BarResizeAction* create(float duration, float targetHeight) {
            auto ret = new BarResizeAction();
            if (ret->initWithDuration(duration, targetHeight)) {
                ret->autorelease();
                return ret;
            }

            delete ret;
            return nullptr;
        }

        bool initWithDuration(float duration, float targetHeight) {
            if (!CCActionInterval::initWithDuration(duration)) return false;

            m_targetHeight = targetHeight;

            return true;
        }

        virtual void startWithTarget(CCNode* target) override {
            CCActionInterval::startWithTarget(target);
            m_startHeight = target->getContentHeight();
            m_deltaHeight = (m_targetHeight) - m_startHeight;
        }

        virtual void update(float time) override {
            if (!this->m_pTarget) return;
            this->m_pTarget->setContentHeight(m_startHeight + m_deltaHeight * time);
        }
};