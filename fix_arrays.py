#!/usr/bin/env python3
import re

# Read the file
with open('/home/engine/project/Axy-GS-8.51-main/VolcanoV2/Volcano-8.51/PlayerBots.h', 'r', encoding='utf-8') as f:
    content = f.read()

# Fix range-based for loops on TArray
# Pattern: for (Type* var : TArrayVar) { ... }
def fix_range_for_loop(match):
    indent = match.group(1)
    var_type = match.group(2)
    var_name = match.group(3)
    array_name = match.group(4)
    loop_body = match.group(5)

    # Create traditional for loop
    traditional_loop = f'''{indent}for (size_t i = 0; i < {array_name}.Num(); i++)
{indent}{{
{indent}    {var_type}* {var_name} = ({var_type}*){array_name}[i];
{loop_body}
{indent}}}'''
    return traditional_loop

# Find and replace range-based for loops
pattern = r'(\s+)for \(([^*]+)\*\s+(\w+)\s+:\s+([\w.]+)\)\s*\{([^}]*(?:\{[^}]*\}[^}]*)*)\}'

content = re.sub(pattern, fix_range_for_loop, content, flags=re.DOTALL)

# Write the fixed content
with open('/home/engine/project/Axy-GS-8.51-main/VolcanoV2/Volcano-8.51/PlayerBots.h', 'w', encoding='utf-8') as f:
    f.write(content)

print("Fixed range-based for loops!")
