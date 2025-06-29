#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class WakaTimeDebugMenu : public Popup<> {
    protected:
        bool setup() override;
    public:
        static WakaTimeDebugMenu* create();
};