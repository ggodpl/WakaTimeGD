# Editor Time Tracker

A Geometry Dash mod for tracking your daily time spent in the editor, with optional [WakaTime](https://wakatime.com) integration.

## Features
- **Automatic time tracking** - Tracks your editing and playtesting time in the background
- **Project breakdowns** – View time spent on each level, with weekly activity charts
- **WakaTime integration** – Send your data to the WakaTime dashboard, just like in your code editor <cr>(currently unsupported on iOS!)</c>

## Setup
1. On first launch, you’ll be asked whether to enable WakaTime integration and auto-installation of the WakaTime CLI
2. If enabled, the mod will download and install the latest version of the CLI automatically.  
   - If you skip auto-installation but still want WakaTime support, follow the **Manual WakaTime Installation** steps below.  
   - You can enable or disable both options anytime in the mod settings.
3. Enter your WakaTime API key in the mod settings. (You can skip this if your key is already set globally by another WakaTime-enabled tool.)
4. That’s it! Time tracking starts automatically. You can open the project tracker from the main menu.  
   Click on a project name to view detailed stats.

## Manual WakaTime Installation
1. Go to [WakaTime CLI releases on GitHub](https://github.com/wakatime/wakatime-cli/releases/latest)
2. Download the version for your operating system.  
   (If you’re unsure which one, check the mod’s debug info – just click the icon in the bottom-right corner of the project list.)
3. Extract the downloaded file
4. Navigate to your user folder:  
   - Windows: `C:/Users/your_username`  
   - macOS: `/Users/your_username`  
   - Linux: `/home/your_username`
6. Move the extracted file into your user folder
7. Relaunch Geometry Dash