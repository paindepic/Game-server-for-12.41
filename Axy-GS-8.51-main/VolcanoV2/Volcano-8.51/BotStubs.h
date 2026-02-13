#pragma once

// BotStubs.h - Minimal stub definitions for missing SDK types required by the Player Bots system
// This file provides forward declarations and minimal implementations for types that exist
// in Spectra 10.40 but not in the 8.51 SDK.

#include "framework.h"

// Forward declarations for external globals used by bot system
inline UAthenaNavSystem* AthenaNavSystem = nullptr;
inline TArray<AActor*> BuildingFoundations;
inline TArray<AActor*> PlayerStarts;
inline UFortBuildingItemDefinition* Floor = nullptr;
inline UFortBuildingItemDefinition* Roof = nullptr;
inline UFortBuildingItemDefinition* Stair = nullptr;
inline UFortBuildingItemDefinition* Wall = nullptr;
inline UFortEditToolItemDefinition* EditTool = nullptr;

// Stub for missing UFortAthenaAIBotCustomizationData class
// This class doesn't exist in 8.51 SDK but is used by the bot system
class UFortAthenaAIBotCustomizationData : public UObject
{
public:
    class UBehaviorTree* BehaviorTree;
    bool bOverrideBehaviorTree;
    bool bOverrideCharacterCustomization;
    bool bOverrideDBNOPlayStyle;
    bool bOverrideSkillLevel;
    bool bOverrideSkillSets;
    bool bOverrideStartupInventory;
    int32 SkillLevel;
    EDBNOPlayStyle DBNOPlayStyle;
    UClass* PawnClass;
    TArray<TEnumAsByte<EFortAthenaAIBehaviorSet>> SkillSets;
    struct FFortAthenaAICharacterCustomization CharacterCustomization;
    class UFortAthenaAIBotInventoryItems* StartupInventory;

    static UClass* StaticClass()
    {
        static UClass* Class = nullptr;
        if (!Class)
            Class = UObject::FindObject<UClass>("FortAthenaAIBotCustomizationData");
        return Class;
    }
};

// Stub for missing UFortAthenaAIBotInventoryItems class
class UFortAthenaAIBotInventoryItems : public UObject
{
public:
    TArray<UFortItemDefinition*> Items;

    static UClass* StaticClass()
    {
        static UClass* Class = nullptr;
        if (!Class)
            Class = UObject::FindObject<UClass>("FortAthenaAIBotInventoryItems");
        return Class;
    }
};

// Forward declaration of FortInventory namespace for GetQuickBars
namespace FortInventory
{
    static EFortQuickBars GetQuickBars(UFortItemDefinition* ItemDefinition)
    {
        if (!ItemDefinition->IsA(UFortWeaponMeleeItemDefinition::StaticClass()) &&
            !ItemDefinition->IsA(UFortEditToolItemDefinition::StaticClass()) &&
            !ItemDefinition->IsA(UFortBuildingItemDefinition::StaticClass()) &&
            !ItemDefinition->IsA(UFortAmmoItemDefinition::StaticClass()) &&
            !ItemDefinition->IsA(UFortResourceItemDefinition::StaticClass()) &&
            !ItemDefinition->IsA(UFortTrapItemDefinition::StaticClass()))
            return EFortQuickBars::Primary;

        return EFortQuickBars::Secondary;
    }
}

// Stub functions for ability system
inline void AbilitySpecConstructor(FGameplayAbilitySpec* Spec, UGameplayAbility* Ability, int32 Level, int32 InputID, UObject* SourceObject)
{
    if (!Spec)
        return;
    
    Spec->Ability = Ability;
    Spec->Level = Level;
    Spec->InputID = InputID;
    Spec->SourceObject = SourceObject;
    // Handle is typically auto-generated
}

inline void GiveAbilityAndActivateOnce(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle* Handle, FGameplayAbilitySpec& Spec)
{
    if (!AbilitySystemComponent)
        return;
    
    // Give the ability
    FGameplayAbilitySpecHandle NewHandle = AbilitySystemComponent->GiveAbility(Spec);
    if (Handle)
        *Handle = NewHandle;
    
    // Try to activate it once
    AbilitySystemComponent->TryActivateAbility(NewHandle, true);
}

// Helper function for creating bot startup inventory
inline void InitBotBuildingItems()
{
    if (!Floor)
        Floor = StaticLoadObject<UFortBuildingItemDefinition>("/Game/Athena/Items/Weapons/Athena_FloorLoot.Athena_FloorLoot");
    if (!Roof)
        Roof = StaticLoadObject<UFortBuildingItemDefinition>("/Game/Athena/Items/Weapons/Athena_RoofLoot.Athena_RoofLoot");
    if (!Stair)
        Stair = StaticLoadObject<UFortBuildingItemDefinition>("/Game/Athena/Items/Weapons/Athena_StairLoot.Athena_StairLoot");
    if (!Wall)
        Wall = StaticLoadObject<UFortBuildingItemDefinition>("/Game/Athena/Items/Weapons/Athena_WallLoot.Athena_WallLoot");
    if (!EditTool)
        EditTool = StaticLoadObject<UFortEditToolItemDefinition>("/Game/Items/Weapons/BuildingTools/EditTool.EditTool");
}

// ImageBase for hooking (used by bot system)
inline uintptr_t ImageBase = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));

// HookVTable template function for virtual table hooking
template<typename T>
void HookVTable(int Index, void* Detour, void** Original)
{
    auto VTable = *reinterpret_cast<void***>(T::StaticClass()->DefaultObject);
    if (!VTable || !VTable[Index])
        return;

    if (Original)
        *Original = VTable[Index];

    DWORD OldProtect;
    VirtualProtect(&VTable[Index], sizeof(void*), PAGE_EXECUTE_READWRITE, &OldProtect);
    VTable[Index] = Detour;
    DWORD Temp;
    VirtualProtect(&VTable[Index], sizeof(void*), OldProtect, &Temp);
}
