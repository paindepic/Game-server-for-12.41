# Bot System Integration for Axy-GS-8.51

## Overview
This document describes the integration of the Player AI Bots system into Axy-GS-8.51, adding 100 intelligent bots that simulate real Battle Royale gameplay.

## Features Added
- **100 AI Bots** that spawn automatically when a match starts
- **Intelligent Behavior**: Bots can loot, fight, build, heal, and move to safe zones
- **Random Bot Names**: Each bot gets a unique name from a pool of 107 names
- **Battle Royale AI**: Bots simulate realistic player behavior including:
  - Landing from the battle bus
  - Looting weapons and items
  - Engaging in combat
  - Building structures
  - Healing when damaged
  - Moving to safe zones
  - Various warmup behaviors (emoting, pickaxing, etc.)

## Files Modified/Added

### Modified Files:
1. **VolcanoV2/Volcano-8.51/dllmain.cpp**
   - Added include for `FortAIBotControllerAthena.h`
   - Added bot system initialization code
   - Loads building item definitions needed for bots

2. **VolcanoV2/Volcano-8.51/framework.h**
   - Added bot system globals (team indices, bot count, etc.)
   - Added global bot system variables (NavSystem, PlayerStarts, BuildingFoundations, etc.)
   - Added building item definition pointers (Wall, Floor, Stair, Roof, EditTool)
   - Added Log helper function

3. **VolcanoV2/Volcano-8.51/PC.h**
   - Added `SpawnBotsThread` function to spawn bots asynchronously
   - Modified `EnterAircraftHook` to trigger bot spawning when match starts
   - Collects PlayerStart locations and BuildingFoundation locations for bot spawning

### Added Files:
1. **VolcanoV2/Volcano-8.51/PlayerBots.h** (94,265 bytes)
   - Main bot AI system with behavior logic
   - Bot states: Warmup, PreBus, Bus, Skydiving, Gliding, Landed, Looting, MovingToSafeZone, Combat, Healing
   - Bot services for combat, looting, healing, and warmup behaviors
   - Item management and inventory systems for bots

2. **VolcanoV2/Volcano-8.51/FortAIBotControllerAthena.h** (18,794 bytes)
   - Bot spawning and customization logic
   - Manages bot controller initialization
   - Handles bot cosmetic loadouts (skins, pickaxes, gliders, etc.)
   - Team assignment logic

3. **VolcanoV2/Volcano-8.51/BotNames.h** (2,094 bytes)
   - Contains 107 unique bot display names
   - Names like "ObsessedTech.", "Timeless.", "LazarLazar.", "SypherPK.", etc.

## Configuration

### Bot Count
You can configure the number of bots to spawn by modifying the `Globals::TotalBotsToSpawn` variable in `framework.h`:
```cpp
inline int TotalBotsToSpawn = 100; // Change this number to spawn more or fewer bots
```

### Team Settings
Bots are assigned to teams automatically. Configure team settings with:
```cpp
inline int MaxPlayersPerTeam = 1;  // For solos (1), duos (2), squads (4)
inline int NextBotTeamIndex = 3;   // Starting team index for bots
```

## How It Works

1. **Initialization** (dllmain.cpp Main function):
   - Building items are loaded (Wall, Floor, Stair, Roof, EditTool)
   - Bot system hooks are initialized
   - System is ready to spawn bots

2. **Match Start** (PC.h EnterAircraftHook):
   - When the first player enters the battle bus, bot spawning is triggered
   - A separate thread (`SpawnBotsThread`) is created

3. **Bot Spawning** (SpawnBotsThread):
   - Collects all PlayerStart actors from the map
   - Collects all BuildingFoundation actors for drop zones
   - Spawns 100 bots with AI controllers
   - Each bot gets:
     - A unique name
     - Random cosmetics (if available)
     - A pickaxe
     - Building tools (wall, floor, stair, roof, edit tool)
     - AI behavior tree (Phoebe behavior tree)

4. **Bot AI** (PlayerBots.h):
   - Bots start in warmup state and perform random warmup actions
   - When the bus launches, bots enter PreBus state
   - Bots jump from the bus and skydive to random locations
   - After landing, bots:
     - Loot nearby items and chests
     - Engage in combat when they see players
     - Heal when damaged
     - Move to safe zones
     - Build structures during combat

## Technical Details

### Bot States
- **Warmup**: Bots emote, look around, or perform other warmup actions
- **PreBus**: Waiting for the battle bus
- **Bus**: On the battle bus
- **Skydiving**: Falling from the bus
- **Gliding**: Using glider to land
- **Landed**: Just landed, transitioning to looting
- **Looting**: Searching for weapons and items
- **MovingToSafeZone**: Running to the safe zone
- **Combat**: Engaged in combat with a player
- **Healing**: Using healing items

### Bot Behavior Services
- **BotsBTService_Warmup**: Handles warmup behaviors
- **BotsBTService_DropFromBus**: Manages bus jumping logic
- **BotsBTService_Loot**: Controls looting behavior
- **BotsBTService_Combat**: Manages combat AI
- **BotsBTService_Healing**: Handles healing item usage

## Known Limitations

1. **Version-Specific Offsets**: Some function hooks are commented out because they were designed for 10.40, not 8.51. The bots still function but may not have all advanced features.

2. **Behavior Tree**: Bots use the Phoebe behavior tree from the game files. If this is not available in 8.51, bots will use fallback behavior.

3. **Pathfinding**: Navigation may not be perfect without proper NavMesh data. Bots can still move and fight but may occasionally get stuck.

## Troubleshooting

### Bots not spawning:
- Check that PlayerStarts exist on the map
- Verify that the Phoebe behavior tree assets are available
- Check console logs for error messages

### Bots behaving oddly:
- This is expected for 8.51 as the system was originally designed for 10.40
- The bots should still be functional for basic gameplay

### Server crashes:
- Reduce the bot count in `Globals::TotalBotsToSpawn`
- Check that all building item definitions loaded correctly

## Credits

- **Bot System**: Based on Spectra-10-40-bots by Timeless/Tech
- **Integration**: Adapted for Axy-GS-8.51
- **Game Server**: Volcano 8.51 (skidded by Axy)

## Future Improvements

- Find correct function offsets for 8.51 to enable all bot features
- Add more bot customization options
- Improve bot combat AI
- Add support for squad/team gameplay
- Optimize performance for 100+ bots
