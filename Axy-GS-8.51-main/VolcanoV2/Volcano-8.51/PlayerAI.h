#pragma once
#include "framework.h"
#include "Inventory.h"
#include <cmath>
#include <ctime>
#include <string>
#include <algorithm>
#include <cstdlib>

namespace PlayerAI
{
    static bool FVector_IsZero(const FVector& V)
    {
        return V.X == 0.0f && V.Y == 0.0f && V.Z == 0.0f;
    }

    static FVector FVector_GetNormalized(const FVector& V)
    {
        float Length = sqrtf(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
        if (Length > 0.0f)
        {
            return FVector{ V.X / Length, V.Y / Length, V.Z / Length };
        }
        return FVector{ 0, 0, 0 };
    }

    static FVector operator-(const FVector& A, const FVector& B)
    {
        return FVector{ A.X - B.X, A.Y - B.Y, A.Z - B.Z };
    }

    static FRotator FVector_ToRotator(const FVector& V)
    {
        FRotator Rot;
        Rot.Yaw = atan2f(V.Y, V.X) * (180.0f / 3.14159265359f);
        Rot.Pitch = atan2f(V.Z, sqrtf(V.X * V.X + V.Y * V.Y)) * (180.0f / 3.14159265359f);
        Rot.Roll = 0.0f;
        return Rot;
    }
    struct AIBotConfig
    {
        FString BotName;
        int SkillLevel;
        bool bCanBuild;
        bool bCanLoot;
        float ReactionTime;
        float Accuracy;
    };

    struct AIBot
    {
        AFortPlayerControllerAthena* Controller;
        AFortPlayerPawnAthena* Pawn;
        AFortPlayerStateAthena* PlayerState;
        AIBotConfig Config;
        FVector TargetLocation;
        AActor* TargetEnemy;
        float NextActionTime;
        float NextJumpTime;
        float NextShootTime;
        float NextBuildTime;
        bool bIsActive;
        bool bHasLandTarget;
        int CurrentWeaponSlot;
    };

    static TArray<AIBot> ActiveBots;
    static bool bAIInitialized = false;
    static float LastTickTime = 0.0f;

    static const wchar_t* BotNames[] = {
        L"Bot_Player_1",
        L"Bot_Player_2",
        L"Bot_Player_3",
        L"Bot_Player_4",
        L"Bot_Player_5",
        L"Bot_Player_6",
        L"Bot_Player_7",
        L"Bot_Player_8",
        L"Bot_Player_9",
        L"Bot_Player_10"
    };

    static AIBotConfig GetDefaultBotConfig()
    {
        AIBotConfig Config;
        Config.BotName = L"AI_Player";
        Config.SkillLevel = 5;
        Config.bCanBuild = true;
        Config.bCanLoot = true;
        Config.ReactionTime = 0.5f;
        Config.Accuracy = 0.7f;
        return Config;
    }

    static FVector GetRandomLocationInSafeZone()
    {
        auto GameState = GetGameState();
        if (!GameState || !GameState->SafeZoneIndicator)
            return FVector{ 0, 0, 1000 };

        FVector SafeZoneCenter = GameState->SafeZoneIndicator->SafeZoneCenter;
        float SafeZoneRadius = GameState->SafeZoneIndicator->SafeZoneRadius;

        float RandomAngle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f;
        float RandomRadius = static_cast<float>(rand()) / RAND_MAX * SafeZoneRadius * 0.8f;

        FVector RandomLocation;
        RandomLocation.X = SafeZoneCenter.X + RandomRadius * cos(RandomAngle);
        RandomLocation.Y = SafeZoneCenter.Y + RandomRadius * sin(RandomAngle);
        RandomLocation.Z = SafeZoneCenter.Z + 10000.0f;

        return RandomLocation;
    }

    static void SetupBotInventory(AFortPlayerControllerAthena* PC)
    {
        if (!PC || !PC->WorldInventory)
            return;

        static auto Wood = UObject::FindObject<UFortItemDefinition>("WoodItemData.WoodItemData");
        static auto Brick = UObject::FindObject<UFortItemDefinition>("StoneItemData.StoneItemData");
        static auto Metal = UObject::FindObject<UFortItemDefinition>("MetalItemData.MetalItemData");

        static auto AR = UObject::FindObject<UFortItemDefinition>("WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03");
        static auto PUMP = UObject::FindObject<UFortItemDefinition>("WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03");
        static auto SMG = UObject::FindObject<UFortItemDefinition>("WID_Pistol_AutoHeavy_Athena_R_Ore_T03.WID_Pistol_AutoHeavy_Athena_R_Ore_T03");
        static auto Medkit = UObject::FindObject<UFortItemDefinition>("Athena_Medkit.Athena_Medkit");
        static auto Shield = UObject::FindObject<UFortItemDefinition>("Athena_Shields.Athena_Shields");

        if (AR) Inventory::AddItem(PC, AR, 1, 30);
        if (PUMP) Inventory::AddItem(PC, PUMP, 1, 5);
        if (SMG) Inventory::AddItem(PC, SMG, 1, 25);
        if (Medkit) Inventory::AddItem(PC, Medkit, 3);
        if (Shield) Inventory::AddItem(PC, Shield, 2);

        if (Wood) Inventory::AddItem(PC, Wood, 500);
        if (Brick) Inventory::AddItem(PC, Brick, 300);
        if (Metal) Inventory::AddItem(PC, Metal, 150);

        static auto YAYA = UObject::FindObject<UAthenaPickaxeItemDefinition>("DefaultPickaxe.DefaultPickaxe");
        auto& test = PC->CosmeticLoadoutPC;
        Inventory::AddItem(PC, test.Pickaxe ? test.Pickaxe->WeaponDefinition : YAYA->WeaponDefinition, 1);
    }

    static AFortPlayerControllerAthena* SpawnBotController(FVector SpawnLocation, FRotator SpawnRotation)
    {
        auto GameMode = GetGameMode();
        auto World = GetWorld();

        if (!GameMode || !World)
            return nullptr;

        FTransform SpawnTransform{};
        SpawnTransform.Translation = SpawnLocation;
        SpawnTransform.Rotation = FRotToQuat(SpawnRotation);
        SpawnTransform.Scale3D = FVector{ 1, 1, 1 };

        auto PlayerController = SpawnActor<AFortPlayerControllerAthena>(AFortPlayerControllerAthena::StaticClass(), SpawnLocation, SpawnRotation);
        if (!PlayerController)
            return nullptr;

        auto PlayerState = (AFortPlayerStateAthena*)PlayerController->PlayerState;
        if (PlayerState)
        {
            PlayerState->bIsBot = true;
            PlayerState->bHasFinishedLoading = true;
            PlayerState->bIsReadyToContinue = true;
            PlayerState->OnRep_bHasFinishedLoading();
        }

        return PlayerController;
    }

    static AFortPlayerPawnAthena* SpawnBotPawn(AFortPlayerControllerAthena* Controller, FVector Location)
    {
        if (!Controller)
            return nullptr;

        auto GameMode = GetGameMode();
        if (!GameMode)
            return nullptr;

        FTransform SpawnTransform{};
        SpawnTransform.Translation = Location;
        SpawnTransform.Scale3D = FVector{ 1, 1, 1 };

        auto Pawn = (AFortPlayerPawnAthena*)GameMode->SpawnDefaultPawnAtTransform(Controller, SpawnTransform);
        if (Pawn)
        {
            Controller->Possess(Pawn);
            Pawn->SetMaxHealth(100);
            Pawn->SetHealth(100);
            Pawn->SetMaxShield(100);
            Pawn->SetShield(100);
        }

        return Pawn;
    }

    static void AssignBotTeam(AFortPlayerStateAthena* BotPlayerState)
    {
        if (!BotPlayerState)
            return;

        auto GameState = GetGameState();
        if (!GameState)
            return;

        int MaxTeam = 2;
        for (int i = 0; i < GameState->PlayerArray.Num(); i++)
        {
            auto PS = Cast<AFortPlayerStateAthena>(GameState->PlayerArray[i]);
            if (PS && PS->TeamIndex > MaxTeam)
                MaxTeam = PS->TeamIndex;
        }

        BotPlayerState->TeamIndex = MaxTeam + 1;
        BotPlayerState->SquadId = BotPlayerState->TeamIndex - 2;
        BotPlayerState->OnRep_PlayerTeam();
        BotPlayerState->OnRep_PlayerTeamPrivate();
        BotPlayerState->OnRep_TeamIndex(0);
        BotPlayerState->OnRep_SquadId();

        GameState->GameMemberInfoArray.MarkArrayDirty();
    }

    static AIBot* SpawnBot(FVector Location = FVector{ 0, 0, 0 }, AIBotConfig* Config = nullptr)
    {
        if (FVector_IsZero(Location))
        {
            Location = GetRandomLocationInSafeZone();
        }

        AIBotConfig BotConfig = Config ? *Config : GetDefaultBotConfig();

        auto Controller = SpawnBotController(Location, FRotator{ 0, 0, 0 });
        if (!Controller)
            return nullptr;

        auto Pawn = SpawnBotPawn(Controller, Location);
        if (!Pawn)
            return nullptr;

        auto PlayerState = (AFortPlayerStateAthena*)Controller->PlayerState;
        if (PlayerState)
        {
            int BotNameIndex = rand() % (sizeof(BotNames) / sizeof(BotNames[0]));
            PlayerState->SetPlayerName(BotNames[BotNameIndex]);
            AssignBotTeam(PlayerState);
        }

        SetupBotInventory(Controller);

        if (Globals::bPlayground)
        {
            Controller->bInfiniteAmmo = true;
            Controller->bBuildFree = true;
            Controller->bInfiniteMagazine = true;
        }

        AIBot NewBot;
        NewBot.Controller = Controller;
        NewBot.Pawn = Pawn;
        NewBot.PlayerState = PlayerState;
        NewBot.Config = BotConfig;
        NewBot.TargetLocation = Location;
        NewBot.TargetEnemy = nullptr;
        NewBot.NextActionTime = GetStatics()->GetTimeSeconds(GetWorld());
        NewBot.NextJumpTime = 0.0f;
        NewBot.NextShootTime = 0.0f;
        NewBot.NextBuildTime = 0.0f;
        NewBot.bIsActive = true;
        NewBot.bHasLandTarget = false;
        NewBot.CurrentWeaponSlot = 0;

        ActiveBots.Add(NewBot);

        LOG_("Spawned AI Bot: {}", BotConfig.BotName.IsValid() ? "AI_Player" : "Unknown");

        return &ActiveBots[ActiveBots.Num() - 1];
    }

    static void RemoveBot(AIBot* Bot)
    {
        if (!Bot || !Bot->bIsActive)
            return;

        if (Bot->Controller)
        {
            Bot->Controller->K2_DestroyActor();
        }

        Bot->bIsActive = false;
    }

    static void UpdateBotMovement(AIBot* Bot, float CurrentTime)
    {
        if (!Bot || !Bot->bIsActive || !Bot->Pawn)
            return;

        if (Bot->Pawn->IsInAircraft())
        {
            if (!Bot->bHasLandTarget && CurrentTime > Bot->NextActionTime)
            {
                Bot->TargetLocation = GetRandomLocationInSafeZone();
                Bot->bHasLandTarget = true;
                Bot->NextActionTime = CurrentTime + 2.0f;
            }

            if (Bot->bHasLandTarget && CurrentTime > Bot->NextActionTime)
            {
                if (Bot->Controller)
                {
                    Bot->Controller->ServerAttemptAircraftJump(FRotator{ 0, 0, 0 });
                }
            }
            return;
        }

        if (CurrentTime < Bot->NextActionTime)
            return;

        if (FVector_IsZero(Bot->TargetLocation) || Bot->Pawn->GetDistanceTo(Bot->TargetLocation) < 100.0f)
        {
            Bot->TargetLocation = GetRandomLocationInSafeZone();
            Bot->NextActionTime = CurrentTime + 5.0f + (static_cast<float>(rand()) / RAND_MAX * 5.0f);
        }

        if (Bot->Pawn)
        {
            FVector Direction = Bot->TargetLocation - Bot->Pawn->K2_GetActorLocation();
            Direction.Z = 0;
            Direction = FVector_GetNormalized(Direction);

            float Speed = 400.0f;
            FVector Velocity = Direction * Speed;
            Bot->Pawn->AddMovementInput(Direction, 1.0f);

            FRotator NewRotation = FVector_ToRotator(Direction);
            Bot->Controller->SetControlRotation(NewRotation);
        }
    }

    static void UpdateBotCombat(AIBot* Bot, float CurrentTime)
    {
        if (!Bot || !Bot->bIsActive || !Bot->Pawn)
            return;

        if (CurrentTime < Bot->NextShootTime)
            return;

        auto GameState = GetGameState();
        if (!GameState)
            return;

        AActor* ClosestEnemy = nullptr;
        float ClosestDist = 2000.0f;

        for (int i = 0; i < GameState->PlayerArray.Num(); i++)
        {
            auto PS = Cast<AFortPlayerStateAthena>(GameState->PlayerArray[i]);
            if (!PS || PS == Bot->PlayerState || PS->bIsBot)
                continue;

            AActor* PSOwner = PS->GetOwner();
            if (!PSOwner)
                continue;

            auto OtherPC = Cast<AFortPlayerControllerAthena>(PSOwner);
            if (!OtherPC)
                continue;

            auto OtherPawn = Cast<AFortPlayerPawnAthena>(OtherPC->Pawn);
            if (!OtherPawn)
                continue;

            float Dist = Bot->Pawn->GetDistanceTo(OtherPawn);
            if (Dist < ClosestDist)
            {
                ClosestDist = Dist;
                ClosestEnemy = OtherPawn;
            }
        }

        Bot->TargetEnemy = ClosestEnemy;

        if (Bot->TargetEnemy && Bot->CurrentWeaponSlot >= 0)
        {
            FVector EnemyLocation = Bot->TargetEnemy->K2_GetActorLocation();
            FVector BotLocation = Bot->Pawn->K2_GetActorLocation();

            FVector Direction = EnemyLocation - BotLocation;
            FRotator LookRotation = FVector_ToRotator(Direction);
            Bot->Controller->SetControlRotation(LookRotation);

            if (Bot->Pawn->CurrentWeapon)
            {
                Bot->Pawn->CurrentWeapon->StartFire(0.0f);
                Bot->Pawn->CurrentWeapon->StopFire(0.1f);
            }

            Bot->NextShootTime = CurrentTime + 0.5f + (static_cast<float>(rand()) / RAND_MAX * 0.5f);
        }
    }

    static void UpdateBotBuilding(AIBot* Bot, float CurrentTime)
    {
        if (!Bot || !Bot->bIsActive || !Bot->Config.bCanBuild)
            return;

        if (CurrentTime < Bot->NextBuildTime)
            return;

        if (Bot->TargetEnemy && (rand() % 100) < 30)
        {
            Bot->NextBuildTime = CurrentTime + 3.0f;
        }
    }

    static void TickBots()
    {
        if (!bAIInitialized)
            return;

        auto World = GetWorld();
        if (!World)
            return;

        float CurrentTime = GetStatics()->GetTimeSeconds(World);
        if (CurrentTime - LastTickTime < 0.1f)
            return;

        LastTickTime = CurrentTime;

        for (int i = 0; i < ActiveBots.Num(); i++)
        {
            AIBot* Bot = &ActiveBots[i];
            if (!Bot->bIsActive)
                continue;

            if (!Bot->Controller || !Bot->Pawn)
            {
                Bot->bIsActive = false;
                continue;
            }

            UpdateBotMovement(Bot, CurrentTime);
            UpdateBotCombat(Bot, CurrentTime);
            UpdateBotBuilding(Bot, CurrentTime);
        }
    }

    static void SpawnBots(int Count)
    {
        for (int i = 0; i < Count; i++)
        {
            SpawnBot();
        }
        LOG_("Spawned {} AI Bots", Count);
    }

    static void ClearAllBots()
    {
        for (int i = 0; i < ActiveBots.Num(); i++)
        {
            RemoveBot(&ActiveBots[i]);
        }
        ActiveBots.Free();
        LOG_("All AI Bots cleared");
    }

    static void Initialize()
    {
        if (bAIInitialized)
            return;

        srand(static_cast<unsigned>(time(nullptr)));
        bAIInitialized = true;

        LOG_("Player AI System Initialized");
    }

    static void Shutdown()
    {
        ClearAllBots();
        bAIInitialized = false;
        LOG_("Player AI System Shutdown");
    }

    static void ProcessCommand(AFortPlayerController* Sender, const FString& Command)
    {
        if (!Sender)
            return;

        std::string Cmd = Command.ToString();
        if (Cmd.find("spawnbots") == 0)
        {
            int Count = 1;
            if (Cmd.length() > 10)
            {
                try
                {
                    Count = std::stoi(Cmd.substr(10));
                }
                catch (...)
                {
                    Count = 1;
                }
            }
            if (Count < 1) Count = 1;
            if (Count > 50) Count = 50;

            SpawnBots(Count);

            if (Sender->MyFortPawn)
            {
                SpawnBot(Sender->MyFortPawn->K2_GetActorLocation() + FVector{ 200, 0, 0 });
            }
        }
        else if (Cmd == "clearbots")
        {
            ClearAllBots();
        }
        else if (Cmd == "aistatus")
        {
            int ActiveCount = 0;
            for (int i = 0; i < ActiveBots.Num(); i++)
            {
                if (ActiveBots[i].bIsActive)
                    ActiveCount++;
            }
            LOG_("Active AI Bots: {}", ActiveCount);
        }
    }
}
