#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class DayLabel : public CCNode {
    protected:
        bool init(const std::string& day, float duration);
    
    public:
        static DayLabel* create(const std::string& day, float duration);
};