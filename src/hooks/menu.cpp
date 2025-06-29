#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include "../wakatime/wakatime.hpp"
#include "../ui/WakaTimeDebugMenu.hpp"
#include "../ui/WakaTimeMenu.hpp"

using namespace geode::prelude;

class $modify(MenuLayerHook, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

        MenuLayerHook::promptOnline();

		auto menuButton = CCMenuItemSpriteExtra::create(
			CircleButtonSprite::createWithSprite("wakatimeLogoGolden.png"_spr, 0.95f, geode::CircleBaseColor::Green, geode::CircleBaseSize::MediumAlt),
			this,
			menu_selector(MenuLayerHook::onMenuButtonPress)
		);

		auto menu = getChildByID("bottom-menu");
		menu->addChild(menuButton);

		menuButton->setID("menu-button"_spr);

		menu->updateLayout();

		return true;
	}

	void onMenuButtonPress(CCObject*) {
		auto menu = WakaTimeMenu::create();
		if (!menu) return;

		menu->setID("wakatime-menu"_spr);
		menu->setZOrder(10);
		menu->show();
	}

    void promptOnline() {
		if (!Mod::get()->hasSavedValue("online-prompt")) {
			auto popup = createQuickPopup("WakaTime", 
				"This mod can track your in-game activity (editing and playtesting) and send it to your WakaTime dashboard\nThis includes information such as level names, the type of activity, and basic metadata (can be configured in mod settings)\n\nIf you prefer not to send any data online, you can use this mod as a local-only time tracker.\n\nThis can be changed at any point in the settings.", 
				"Disable", 
				"Enable",
				[this](bool btn1, bool btn2) {
					if (btn1) {
						Mod::get()->setSavedValue("online-prompt", true);
						Mod::get()->setSettingValue("online", false);
						Mod::get()->setSettingValue("auto-download", false); // safe to assume that if someone doesn't want their data sent to wakatime, they don't want wakatime
					}
					if (btn2) {
						Mod::get()->setSavedValue("online-prompt", true);
						Mod::get()->setSettingValue("online", true);
						MenuLayerHook::promptAutoDownload();
					}
				},
				false
			);

			popup->m_scene = this;
			popup->show();
		} else {
            promptAutoDownload();
        }
    }

    void promptAutoDownload() {
		if (!Mod::get()->hasSavedValue("auto-download-prompt")) {
			auto popup = createQuickPopup("WakaTime", 
				"To send activity to WakaTime, this mod uses the official wakatime-cli tool. It can automatically download and update this tool for your system.\n\nEnabling will allow automatic downloads.\nAlternatively, you can manage it manually (more info in the mod's README).\n\nThis can be changed at any point in the settings.", 
				"Disable",
				"Enable",
				[](bool btn1, bool btn2) {
					if (btn1) {
						Mod::get()->setSavedValue("auto-download-prompt", true);
						Mod::get()->setSettingValue("auto-download", false);
					}
					if (btn2) {
						Mod::get()->setSavedValue("auto-download-prompt", true);
						Mod::get()->setSettingValue("auto-download", true);
                        wakatime::refetch();
					}
				},
				false
			);

			popup->m_scene = this;
			popup->show();
		}
    }
};