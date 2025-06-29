#include <fmt/format.h>
#include "WakaTimeDebugMenu.hpp"
#include "../wakatime/cli.hpp"
#include "../wakatime/wakatime.hpp"
#include "../utils/utils.hpp"

bool WakaTimeDebugMenu::setup() {
    setTitle("WakaTime");
    setID("WakaTimeDebugMenu");

    std::string content = fmt::format(
        "CLI Status: {}\n\n"
        "CLI Path:  `{}`\n\n"
        "CLI Version: {}\n\n"
        "Last Heartbeat: {}\n\n"
        "Total local time: {}\n\n",
        cli::isInstalled() ? "<cg>Installed</c>" : "<cr>Not installed</c>",
        cli::getPath().filename().string(),
        fmt::format("{}{}</c>", cli::isLatest() ? "<cg>" : "<cy>", cli::getVersion()),
        ::utils::ago(wakatime::getLastHeartbeat()),
        Mod::get()->getSavedValue<int>("timeTrackingTotal")
    );

    auto textArea = MDTextArea::create(content, { m_mainLayer->getContentWidth() - 20, m_mainLayer->getContentHeight() - 60 });
    textArea->setID("debugInfo");
    textArea->setPosition({m_mainLayer->getContentWidth() / 2, m_mainLayer->getContentHeight() / 2 - 10});
    textArea->setAnchorPoint({ 0.5f, 0.5f });

    m_mainLayer->addChild(textArea);

    return true;
}

WakaTimeDebugMenu* WakaTimeDebugMenu::create() {
    auto ret = new WakaTimeDebugMenu();
    if (ret->initAnchored(300.f, 180.f, "wakatimeMenuBackground.png"_spr)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}