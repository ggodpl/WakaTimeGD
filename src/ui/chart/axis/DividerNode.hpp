#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class DividerNode : public CCDrawNode {
    public:
        static DividerNode* create();

        void drawDivider();

        void setPositionEase(const CCPoint& point);
};