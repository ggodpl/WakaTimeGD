#include <Geode/Geode.hpp>
#include "wakatime/wakatime.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
	#ifndef GEODE_IS_IOS
	wakatime::init();
	#endif
}