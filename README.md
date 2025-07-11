# WakaTime GD

A Geometry Dash mod that integrates with [WakaTime](https://wakatime.com) to automatically track time spent in the level editor.

## Features
- **Automatic time tracking** - Tracks editing and playtesting sessions in the background
- **Local statistics** - View weekly and all-time project breakdowns
- **WakaTime integration** - Optional sync with the WakaTime dashboard

## Setup

1. Install the mod via Geode
2. On the first launch, choose whether to enable WakaTime integration
3. If enabled, the mod will automatically download the required WakaTime CLI (if you don't want that, disable it and follow Manual CLI Installation instructions below)
4. Optionally configure your WakaTime API key and URL in mod settings

## Usage

- Time tracking is done automatically when entering the level editor
- You can view your statistics through the WakaTime button in the main menu

## Manual CLI Installation

If auto-download is disabled, you need to install `wakatime-cli` manually (unless it's already present, in which case you can skip this step entirely):
- Download from [wakatime-cli releases](https://github.com/wakatime/wakatime-cli/releases/latest)
- Extract the downloaded archive
- Place the `wakatime-cli` executable in `~/.wakatime/` (Linux/Mac) or `%USERPROFILE%\.wakatime\` (Windows)
- Alternatively, ensure the executable is available in your system PATH