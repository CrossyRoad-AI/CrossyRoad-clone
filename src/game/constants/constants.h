#ifndef CONSTANTS_H
#define CONSTANTS_H

// Shared memory
static const char MEMORY_MAP_NAME[] = "Global\\crossyroad-memorymap";

// General game consts
static const unsigned int FURTHEST_GENERATED_ROW = 15;
static const unsigned int ROWS_BEHIND_PLAYER = 5 + 3;

// Row types
static const unsigned int ROW_GRASS = 0x1 << 4;
static const unsigned int ROW_WATER = 0x2 << 4;

// Grass specific rows
static const unsigned int ROW_GRASS_EMPTY = 0x0;
static const unsigned int ROW_GRASS_FULL_TREE = 0x1;
static const unsigned int ROW_GRASS_FEW_TREES_RANDOM = 0x2;
static const unsigned int ROW_GRASS_ALOT_TREES_RANDOM = 0x3;

// Water specific rows
static const unsigned int ROW_WATER_ALOT_WATER_LILLY = 0x0;
static const unsigned int ROW_WATER_FEW_WATER_LILLY = 0x1;

#endif