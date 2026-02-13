#!/usr/bin/env python3
import re

# Read the file
with open('/home/engine/project/Axy-GS-8.51-main/VolcanoV2/Volcano-8.51/PlayerBots.h', 'r', encoding='utf-8') as f:
    content = f.read()

# Fix SpawnPickup calls with 7 arguments (remove the last argument)
# Pattern: SpawnPickup(..., bot->Pawn)
pattern = r'SpawnPickup\(([^,]+,\s*[^,]+,\s*[^,]+,\s*[^,]+,\s*[^,]+,\s*[^,]+,\s*)bot->Pawn\s*\)'

def fix_spawnpickup(match):
    return f'SpawnPickup({match.group(1)})'

content = re.sub(pattern, fix_spawnpickup, content)

# Write the fixed content
with open('/home/engine/project/Axy-GS-8.51-main/VolcanoV2/Volcano-8.51/PlayerBots.h', 'w', encoding='utf-8') as f:
    f.write(content)

print("Fixed SpawnPickup calls!")
