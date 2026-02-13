# How to Use the Bot System

## Quick Start

The bot system is fully integrated and will automatically spawn bots when a match starts. No additional configuration is required for basic functionality!

## Basic Usage

1. **Compile the project** using Visual Studio 2019 or later
2. **Inject the DLL** into the Fortnite game server
3. **Start a match** - bots will automatically spawn when the first player enters the battle bus
4. **Watch the console** for bot spawn messages and status updates

## Configuration

### Changing the Number of Bots

Open `VolcanoV2/Volcano-8.51/framework.h` and modify this line:

```cpp
inline int TotalBotsToSpawn = 100; // Change this number
```

**Recommended values:**
- **25-50 bots**: For testing or lower-end hardware
- **100 bots** (default): For full Battle Royale experience
- **150+ bots**: For chaos (may cause performance issues)

### Changing Team Settings

For different game modes (Solos, Duos, Squads):

```cpp
inline int MaxPlayersPerTeam = 1;  // 1 = Solos, 2 = Duos, 4 = Squads
```

### Adjusting Bot Spawn Timing

In `VolcanoV2/Volcano-8.51/PC.h`, in the `SpawnBotsThread` function:

```cpp
Sleep(3000); // Wait 3 seconds before spawning - adjust as needed
```

## Console Output

When bots are spawning, you'll see console messages like:

```
VolcanoV2 : Initializing Bot System...
VolcanoV2 : Building items loaded!
VolcanoV2 : Bot system hooks ready (8.51 mode)!
VolcanoV2 : Bot system initialized! Ready to spawn 100 bots!
VolcanoV2 : Creating bot spawn thread...
VolcanoV2 : Starting bot spawn thread...
VolcanoV2 : Found 42 PlayerStart locations
VolcanoV2 : Found 156 Building Foundations for drop zones
VolcanoV2 : Spawned 10 bots so far...
VolcanoV2 : Spawned 20 bots so far...
...
VolcanoV2 : Successfully spawned 100 AI bots!
```

## Webhook Notifications

If you have Discord webhooks configured, you'll receive notifications when:
1. The match starts
2. The bots have been spawned
3. The battle bus launches

## Customizing Bot Behavior

### Bot Names

Edit `VolcanoV2/Volcano-8.51/BotNames.h` to add or modify bot names:

```cpp
inline std::vector<std::string> BotDisplayNames = {
    "YourBotName1",
    "YourBotName2",
    "CoolBot123",
    // Add more names here
};
```

### Bot State Timing

In `VolcanoV2/Volcano-8.51/PlayerBots.h`, you can adjust various timings:

```cpp
// Example: Time until bot drops from bus
Aircraft->DropStartTime = GetStatics()->GetTimeSeconds(GetWorld()) + 5; // 5 seconds
```

### Bot Combat Settings

Adjust bot combat behavior in the `BotsBTService_Combat` class:

```cpp
// Aim accuracy randomization
float RandomXmod = Math->RandomFloatInRange(-150, 150); // Increase for less accuracy
float RandomYmod = Math->RandomFloatInRange(-150, 150);
float RandomZmod = Math->RandomFloatInRange(-150, 150);
```

## Troubleshooting

### Bots not spawning

**Problem:** No bots appear in the match
**Solutions:**
1. Check console for error messages
2. Verify PlayerStarts exist on the map
3. Make sure `Globals::TotalBotsToSpawn` is set correctly
4. Try reducing the bot count to 10 for testing

### Server crashes when bots spawn

**Problem:** Server crashes during bot spawn
**Solutions:**
1. Reduce `TotalBotsToSpawn` to 50 or 25
2. Increase the sleep time in `SpawnBotsThread` (e.g., `Sleep(200)` between every 10 bots)
3. Check that all building item definitions are loading correctly

### Bots standing still

**Problem:** Bots spawn but don't move
**Solutions:**
1. This is expected behavior if they're in Warmup state
2. Wait for the battle bus to start
3. Check console for "ServerBotManager spawn failed" messages

### Bots have no cosmetics

**Problem:** All bots look the same
**Solutions:**
1. This is normal - the bot system will use default skins
2. To add cosmetics, you'd need to populate the `Characters`, `Pickaxes`, `Gliders`, etc. arrays in `FortAIBotControllerAthena.h`

## Advanced Configuration

### Loading Bot Cosmetics

Add code to load skins and cosmetics (in `dllmain.cpp` after bot initialization):

```cpp
// Example: Load bot skins
FortAIBotControllerAthena::Characters.push_back(
    StaticLoadObject<UAthenaCharacterItemDefinition>("/Path/To/Skin.Skin")
);

// Example: Load bot pickaxes
FortAIBotControllerAthena::Pickaxes.push_back(
    StaticLoadObject<UAthenaPickaxeItemDefinition>("/Path/To/Pickaxe.Pickaxe")
);
```

### Changing Bot Skill Level

In `FortAIBotControllerAthena.h`, modify:

```cpp
BotCustomizationData->SkillLevel = 1; // 1-10 (higher = harder)
```

### Enabling Debug Logging

To see more detailed bot behavior logs, uncomment Log statements in `PlayerBots.h`:

```cpp
Log(std::format("Bot state: {}", (int)BotState).c_str());
```

## Performance Tips

1. **Lower bot count**: 50-75 bots instead of 100
2. **Increase spawn delays**: Add more `Sleep()` calls in `SpawnBotsThread`
3. **Disable cosmetics**: Keep the cosmetic arrays empty to save memory
4. **Optimize tick rate**: Bots tick every frame, consider reducing update frequency

## Known Issues

1. **Pathfinding**: Bots may occasionally get stuck without proper NavMesh
2. **Building**: Bot building is basic and may not always be optimal
3. **Version compatibility**: Some features work better in 10.40 than 8.51
4. **Performance**: 100+ bots may cause performance issues on some hardware

## Support

For issues or questions:
- Check the console output for error messages
- Review `BOT_INTEGRATION.md` for detailed technical information
- Reduce bot count to isolate issues
- Check GitHub issues (if available)

## Example Configurations

### Low-End Server (25 Bots)
```cpp
inline int TotalBotsToSpawn = 25;
inline int MaxPlayersPerTeam = 1;
```

### Standard Battle Royale (100 Bots)
```cpp
inline int TotalBotsToSpawn = 100;
inline int MaxPlayersPerTeam = 1;
```

### Duos with Bots (50 Bot Teams)
```cpp
inline int TotalBotsToSpawn = 100; // 50 teams of 2
inline int MaxPlayersPerTeam = 2;
```

### Chaos Mode (150 Bots)
```cpp
inline int TotalBotsToSpawn = 150;
inline int MaxPlayersPerTeam = 1;
// Note: May cause performance issues!
```

## Credits

Bot system adapted from Spectra-10-40-bots for use in Axy-GS-8.51.
