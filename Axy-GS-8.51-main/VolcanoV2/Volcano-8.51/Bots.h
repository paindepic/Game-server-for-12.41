#pragma once
#include "framework.h"
#include "Inventory.h"
#include "Abilites.h"
#include <vector>
#include <string>

namespace Bots
{
    // Bot names for variety
    static const std::vector<std::wstring> BotNames = {
        L"ShadowOps",
        L"DarkVoyager",
        L"Omega",
        L"SkullTrooper",
        L"GhoulTrooper",
        L"Raptor",
        L"Drift",
        L"BriteBomber",
        L"CuddleTeamLeader",
        L"Fishstick",
        L"Peely",
        L"Lynx",
        L"Spectrum",
        L"BlackKnight",
        L"SkullTrooper",
        L"RedKnight",
        L"MogulMaster",
        L"Leviathan",
        L"SparkleSpecialist",
        L"BriteGunner",
        L"Dusty",
        L"DimensionalInfiltrator",
        L"Galaxy",
        L"Rogue",
        L"Slushy",
        L"Vector",
        L"Fate",
        L"BunkerBasin",
        L"BrilliantStriker",
        L"GoalStorm",
        L"TopNotch",
        L"WildCard",
        L"Abstrakt",
        L"Junction",
        L"BaoBrothers",
        L"Pepper",
        L"Teaser",
        L"SgtWinter",
        L"ChaosAgent",
        L"Meowscles",
        L"Deadfire",
        L"Marauder",
        L"Gunnar",
        L"Fenix",
        L"Wrath",
        L"Fade",
        L"Reaper",
        L"DarkBomber",
        L"TomatoHead",
        L"Al",
        L"DurrBurger",
        L"FishThicc",
        L"PeelyBone",
        L"AgentPeely",
        L"BananaSplit",
        L"DJYonder",
        L"Lucia",
        L"Vega",
        L"Raymond",
        L"TresChez",
        L"Fishstick",
        L"ChompSki",
        L"MechaTeamLeader",
        L"Chromatica",
        L"Amara",
        L"Konda",
        L"Jules",
        L"Kit",
        L"Midas",
        L"Orelia",
        L"ScarletShadow",
        L"RogueAgent",
        L"Brat",
        L"Tina",
        L"Menace",
        L"FabioSparklemane",
        L"Kyra",
        L"Kondor",
        L"Ragsy",
        L"Mandolin",
        L"Peely",
        L"BaoBrothers",
        L"PurpleSkullTrooper",
        L"BriteBomber",
        L"SkullTrooper",
        L"RedKnight",
        L"MogulMaster",
        L"CuddleTeamLeader",
        L"Fishstick",
        L"Peely",
        L"Drift",
        L"Omega",
        L"Raptor",
        L"GhoulTrooper",
        L"DarkVoyager",
        L"ShadowOps",
        L"BlackKnight",
        L"Spectrum",
        L"Lynx",
        L"SparkleSpecialist",
        L"BriteGunner"
    };

    // Global bot tracking
    static TArray<AFortAthenaAIBotController*> AliveBots;
    static int TotalBotsSpawned = 0;
    static const int MaxBots = 100;

    // Get a random bot name
    static std::wstring GetRandomBotName()
    {
        if (BotNames.empty())
            return L"Bot";
        
        int index = rand() % BotNames.size();
        return BotNames[index];
    }

    // Spawn a single bot
    static AFortAthenaAIBotController* SpawnBot(const std::wstring& BotName, FVector SpawnLocation, uint8 TeamIndex)
    {
        auto GameMode = GetGameMode();
        if (!GameMode)
        {
            LOG_("GameMode is null, cannot spawn bot!");
            return nullptr;
        }

        auto GameState = GetGameState();
        if (!GameState)
        {
            LOG_("GameState is null, cannot spawn bot!");
            return nullptr;
        }

        // Spawn the AI Controller
        auto BotController = SpawnActor<AFortAthenaAIBotController>(AFortAthenaAIBotController::StaticClass());
        if (!BotController)
        {
            LOG_("Failed to spawn bot controller!");
            return nullptr;
        }

        // Spawn the Player Pawn
        auto PawnClass = GameMode->DefaultPawnClass;
        if (!PawnClass)
        {
            PawnClass = APlayerPawn_Athena_C::StaticClass();
        }

        auto Pawn = SpawnActor<APlayerPawn_Athena_C>((UClass*)PawnClass, SpawnLocation);
        if (!Pawn)
        {
            LOG_("Failed to spawn bot pawn!");
            BotController->K2_DestroyActor();
            return nullptr;
        }

        // Possess the pawn
        BotController->Possess(Pawn);
        BotController->PlayerBotPawn = Pawn;

        // Set up PlayerState
        auto PlayerState = (AFortPlayerStateAthena*)BotController->PlayerState;
        if (PlayerState)
        {
            // Set bot name
            PlayerState->PlayerName = BotName.c_str();
            PlayerState->OnRep_PlayerName();

            // Set team
            PlayerState->TeamIndex = TeamIndex;
            PlayerState->SquadId = TeamIndex - 2;
            PlayerState->OnRep_TeamIndex(0);
            PlayerState->OnRep_PlayerTeam();
            PlayerState->OnRep_PlayerTeamPrivate();
            PlayerState->OnRep_SquadId();

            // Mark as bot
            PlayerState->bIsBot = true;

            // Grant abilities
            GrantAbilitySet(PlayerState);

            // Initialize cosmetic loadout with random skin
            static auto DefaultCharacter = UObject::FindObject<UAthenaCharacterItemDefinition>("/Game/Athena/Items/Cosmetics/Characters/CID_002_Athena_Commando_F.CID_002_Athena_Commando_F");
            
            // Set a random character
            BotController->CustomizationLoadout.Character = DefaultCharacter;
            if (DefaultCharacter && DefaultCharacter->HeroDefinition)
            {
                Pawn->CosmeticLoadout = BotController->CustomizationLoadout;
                Pawn->OnRep_CosmeticLoadout();

                PlayerState->HeroType = DefaultCharacter->HeroDefinition;
                PlayerState->OnRep_HeroType();
            }
        }

        // Add starting items
        for (int i = 0; i < GameMode->StartingItems.Num(); i++)
        {
            // Bots need inventory setup
            // For now, we'll equip the pickaxe
        }

        static auto DefaultPickaxe = UObject::FindObject<UAthenaPickaxeItemDefinition>("DefaultPickaxe.DefaultPickaxe");
        if (DefaultPickaxe)
        {
            // Give pickaxe to bot
        }

        // Add to alive bots tracking
        AliveBots.Add(BotController);
        GameState->AlivePlayers.Add(BotController);
        TotalBotsSpawned++;

        LOG_("Spawned bot: {} with TeamIndex: {}", std::string(BotName.begin(), BotName.end()), TeamIndex);

        return BotController;
    }

    // Spawn multiple bots for Battle Royale
    static void SpawnBotsForBR(int BotCount = MaxBots)
    {
        auto GameMode = GetGameMode();
        auto GameState = GetGameState();
        if (!GameMode || !GameState)
        {
            LOG_("Cannot spawn bots: GameMode or GameState is null!");
            return;
        }

        LOG_("Spawning {} bots for Battle Royale...", BotCount);

        // Calculate spawn locations around the map
        // For simplicity, we'll use safe zone locations as spawn points
        TArray<FVector> SpawnLocations;
        
        if (GameMode->SafeZoneLocations.Num() > 0)
        {
            SpawnLocations = GameMode->SafeZoneLocations;
        }
        else
        {
            // Fallback to some default locations
            SpawnLocations.Add(FVector{ 0, 0, 1000 });
            SpawnLocations.Add(FVector{ 5000, 0, 1000 });
            SpawnLocations.Add(FVector{ -5000, 0, 1000 });
            SpawnLocations.Add(FVector{ 0, 5000, 1000 });
            SpawnLocations.Add(FVector{ 0, -5000, 1000 });
        }

        // Seed RNG
        srand(static_cast<unsigned>(time(nullptr)));

        // Spawn bots
        for (int i = 0; i < BotCount; i++)
        {
            // Get random spawn location
            FVector SpawnLoc = SpawnLocations[i % SpawnLocations.Num()];
            
            // Add some randomness to spawn location
            SpawnLoc.X += (rand() % 10000) - 5000;
            SpawnLoc.Y += (rand() % 10000) - 5000;
            SpawnLoc.Z = 15000; // Spawn in the air so they can drop

            // Assign team (random team for FFA, or organized for squads)
            uint8 TeamIndex = 2 + (i % 50); // Teams 2-51

            // Get random bot name
            std::wstring BotName = GetRandomBotName();

            // Spawn the bot
            SpawnBot(BotName, SpawnLoc, TeamIndex);

            // Small delay between spawns to avoid overwhelming the system
            Sleep(10);
        }

        LOG_("Finished spawning {} bots!", BotCount);
    }

    // Remove bot from alive tracking
    static void OnBotDeath(AFortAthenaAIBotController* Bot)
    {
        if (!Bot)
            return;

        for (int i = 0; i < AliveBots.Num(); i++)
        {
            if (AliveBots[i] == Bot)
            {
                AliveBots.Remove(i);
                break;
            }
        }

        auto GameState = GetGameState();
        if (GameState)
        {
            for (int i = 0; i < GameState->AlivePlayers.Num(); i++)
            {
                if (GameState->AlivePlayers[i] == Bot)
                {
                    GameState->AlivePlayers.Remove(i);
                    break;
                }
            }
        }

        LOG_("Bot removed from alive tracking. Remaining bots: {}", AliveBots.Num());
    }

    // Initialize bot system
    static void Initialize()
    {
        LOG_("Initializing Bot System...");
        AliveBots.Free();
        TotalBotsSpawned = 0;
        LOG_("Bot System initialized!");
    }

    // Cleanup bot system
    static void Cleanup()
    {
        LOG_("Cleaning up Bot System...");
        AliveBots.Free();
        TotalBotsSpawned = 0;
        LOG_("Bot System cleaned up!");
    }
}
