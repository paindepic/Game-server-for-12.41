#!/usr/bin/env python3
import re

# Read the file
with open('/home/engine/project/Axy-GS-8.51-main/VolcanoV2/Volcano-8.51/PlayerBots.h', 'r', encoding='utf-8') as f:
    content = f.read()

# Find all SpawnPickup calls
matches = re.findall(r'SpawnPickup\([^)]+\)', content)

print("Found SpawnPickup calls:")
for i, match in enumerate(matches, 1):
    print(f"{i}. {match}")

# Check for calls with 7 arguments (incorrect)
# Correct signature should be:
# SpawnPickup(UFortItemDefinition* ItemDef, int OverrideCount, int LoadedAmmo, FVector Loc, EFortPickupSourceTypeFlag SourceType, EFortPickupSpawnSource Source)
# or
# SpawnPickup(FFortItemEntry& ItemEntry, FVector Loc, EFortPickupSourceTypeFlag SourceType, EFortPickupSpawnSource Source, int OverrideCount = -1)

for i, match in enumerate(matches, 1):
    args = match[12:-1].split(',')
    print(f"SpawnPickup call {i} has {len(args)} arguments")
