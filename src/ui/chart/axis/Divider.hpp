#pragma once
#include "DividerNode.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Divider : public CCNode {
    protected:
        int m_count;
        std::vector<float> m_positions;
        bool init(int count);

    public:
        static Divider* create(int count);

        void setCount(int count) {
            m_count = count;
        }
        void drawDividers();
};