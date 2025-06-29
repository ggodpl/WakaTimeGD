#include <Geode/Geode.hpp>
#include "wakatime/wakatime.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	wakatime::init();
}