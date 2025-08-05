#pragma once
#include <Geode/Geode.hpp>
#include <functional>

using namespace geode::prelude;

class WeekSelector : public CCNode {
    protected:
        int m_offset;
        int m_prevOffset;
        int m_max;

        CCMenuItemSpriteExtra* m_prev;
        CCMenuItemSpriteExtra* m_next;
        CCMenu* m_menu;

        std::function<void()> m_callback;

        bool init(float width, int max);
    public:
        static WeekSelector* create(float width, int max);
        void updateCurrent();

        void bind(std::function<void()> callback) {
            m_callback = callback;
        }

        void setMax(int max) {
            m_max = max;
            updateCurrent();
        }

        void setOffset(int offset) {
            m_offset = offset;
            updateCurrent();
        }
        
        int getOffset() const {
            return m_offset;
        }

        void prev(CCObject*) {
            m_prevOffset = m_offset;
            int newOffset = m_offset + 1;
            if (newOffset > m_max) newOffset = m_max;
            m_offset = newOffset;
            updateCurrent();

            if (m_callback) m_callback();
        }
        
        void next(CCObject*) {
            m_prevOffset = m_offset;
            int newOffset = m_offset - 1;
            if (newOffset < 0) newOffset = 0;
            m_offset = newOffset;
            updateCurrent();

            if (m_callback) m_callback();
        }
};