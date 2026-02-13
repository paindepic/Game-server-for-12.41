# Game-server-for-12.41
Axy-GS-8.51-main Project Reboot v8.51 - A Fortnite Season 8.51 game server with integrated player bots system.

## Player Bots Integration
The player bots system from Spectra 10.40 has been successfully integrated into the server codebase. See [BOTS_INTEGRATION.md](./BOTS_INTEGRATION.md) for detailed documentation.

## Project Structure
- `Axy-GS-8.51-main/` - Main server codebase (Volcano 8.51 based)
  - `VolcanoV2/Volcano-8.51/` - Core server implementation
    - `BotNames.h` - Bot display names
    - `FortAIBotControllerAthena.h` - Bot controller and spawning
    - `PlayerBots.h` - Bot AI and behavior logic

## Build Instructions
This is a Visual Studio C++ project targeting Windows. Open `Axy-GS-8.51-main/VolcanoV2/Volcano-8.51.sln` in Visual Studio to build.

## Features
See `Axy-GS-8.51-main/Features.md` for a complete list of features.

