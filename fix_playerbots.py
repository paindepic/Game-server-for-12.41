#!/usr/bin/env python3
import re

# Read the file
with open('/home/engine/project/Axy-GS-8.51-main/VolcanoV2/Volcano-8.51/PlayerBots.h', 'r', encoding='utf-8') as f:
    content = f.read()

# Fix 1: Remove PreBus from EBotState enum
content = content.replace('        PreBus,\n', '')

# Fix 2: Replace UKismetMathLibrary::GetDefaultObj() with GetMath()
content = content.replace('UKismetMathLibrary::GetDefaultObj()', 'GetMath()')

# Fix 3: Replace UGameplayStatics::GetDefaultObj() with GetStatics()
content = content.replace('UGameplayStatics::GetDefaultObj()', 'GetStatics()')

# Fix 4: Replace UGameplayStatics::GetAllActorsOfClass with GetStatics()->GetAllActorsOfClass
content = content.replace('UGameplayStatics::GetAllActorsOfClass', 'GetStatics()->GetAllActorsOfClass')

# Fix 5: Replace UWorld::GetWorld() with GetWorld()
content = content.replace('UWorld::GetWorld()', 'GetWorld()')

# Fix 6: Replace GameMode->AliveBots with Globals::AliveBots
content = content.replace('GameMode->AliveBots', 'Globals::AliveBots')

# Fix 7: Fix Log() calls that use std::string - add .c_str()
content = re.sub(r'Log\(([^)]+)\)', r'Log((\1).c_str())', content)

# Write the fixed content
with open('/home/engine/project/Axy-GS-8.51-main/VolcanoV2/Volcano-8.51/PlayerBots.h', 'w', encoding='utf-8') as f:
    f.write(content)

print("PlayerBots.h fixed successfully!")
