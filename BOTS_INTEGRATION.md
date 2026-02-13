# Player Bots Integration Summary

## Overview
This document summarizes the integration of the Player Bots System from Spectra 10.40 into the Axy-GS-8.51-main Project Reboot v8.51 server codebase.

## Files Integrated

### Copied Files
The following bot system files were copied from `Spectra-10-40-bots-main/` to `Axy-GS-8.51-main/VolcanoV2/Volcano-8.51/`:

1. **BotNames.h** (2,094 bytes)
   - Contains a list of bot display names for player bots

2. **FortAIBotControllerAthena.h** (18,794 bytes)
   - Main bot controller implementation
   - Handles bot spawning, customization, and team assignment
   - Contains hooks for bot behavior tree and pathfinding

3. **PlayerBots.h** (94,265 bytes)
   - Core bot logic and state machine
   - Enums for bot states, looting filters, and behaviors
   - Bot AI behavior implementation

## Project Files Modified

### Volcano-8.51.vcxproj
Added the three bot header files to the `<ClInclude>` section for compilation.

### Volcano-8.51.vcxproj.filters
Added the three bot header files to the "GameServer" filter in Visual Studio.

### framework.h
Added necessary globals and macros to support the bot system:

```cpp
// Logging macro for bot system
#define Log(...) { std::cout << "VolcanoV2 : " << __VA_ARGS__ << std::endl; AAAA << __VA_ARGS__ << std::endl; }

// Base address for hooks
static __int64 ImageBase = __int64(GetModuleHandleW(0));

// Bot-related globals
static TArray<AActor*> PlayerStarts{};
static UFortWeaponItemDefinition* Floor = nullptr;
static UFortWeaponItemDefinition* Roof = nullptr;
static UFortWeaponItemDefinition* Stair = nullptr;
static UFortWeaponItemDefinition* Wall = nullptr;
static UFortWeaponItemDefinition* EditTool = nullptr;
```

### Globals Namespace (in framework.h)
Added bot team management variables:

```cpp
// Bot team management
int NextBotTeamIndex = 4;
int CurrentBotsOnTeam = 0;
int MaxPlayersPerTeam = 1;
```

## Dependencies

The bot system has the following dependencies which are already present in the codebase:
- framework.h ✓
- Looting.h ✓
- UE SDK (sdk/SDK.hpp) ✓
- MinHook library ✓

## Compilation Status

All necessary files have been integrated and project files updated. The bot system should now compile successfully with the existing codebase.

## Next Steps (for future implementation)

To fully activate the bot system, you will need to:

1. **Initialize the bot hooks** in dllmain.cpp:
   ```cpp
   #include "FortAIBotControllerAthena.h"
   
   // In the Main() function or appropriate initialization function:
   FortAIBotControllerAthena::HookAll();
   ```

2. **Populate PlayerStarts array** with valid spawn locations from the level

3. **Initialize building item definitions** (Floor, Roof, Stair, Wall, EditTool) with actual item definitions

4. **Call SpawnPlayerBot()** when you want to spawn bots:
   ```cpp
   FortAIBotControllerAthena::SpawnPlayerBot();
   ```

5. **Initialize bot tick logic** if using custom bot behavior

## Notes

- The bot system uses Phoebe behavior tree from Fortnite's AI system
- Bots are assigned random names from BotDisplayNames vector
- Team assignment follows the existing team management system
- Pathfinding is currently disabled in the hooks (commented out)

## Credits

- Original bot code by: techs skunky (Spectra 10.40)
- Integration for: Axy-GS-8.51-main Project Reboot v8.51
