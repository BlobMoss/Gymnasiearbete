#pragma once

// Blocks:

#define EMPTY 0
#define PLANKS 1
#define SAND 2
#define GRASS 3
#define LOG 4
#define STONE 5
#define IRONORE 6
#define GOLDORE 7
#define FERN 8
#define TREE 9

// Items:

#define CANNONBALL 17
#define BULLET 18
#define CUTLASS 19
#define FLINTLOCK 20
#define CLOTH 21
#define IRON 22
#define GOLD 23
#define FIBRE 24
#define MAST 25
#define HELM 26
#define CANNON 27

const float isStackable[] =
{
    true, // NaN
    true, // 1
    true, // 2
    true, // 3
    true, // 4
    true, // 5
    true, // 6
    true, // 7
    true, // 8
    true, // 9
    true, // 10
    true, // 11
    true, // 12
    true, // 13
    true, // 14
    true, // 15
    true, // 16
    true, // 17
    true, // 18
    false, // 19
    false, // 20
    true, // 21
    true, // 22
    true, // 23
    true, // 24
    false, // 25
    false, // 26
    false, // 27
    true, // 28
    true, // 29
    true, // 30
    true, // 31
    true  // 32
};

const float breakTimes[] =
{
    0.0f, // 0
    1.0f, // 1
    2.5f, // 2
    2.5f, // 3
    2.0f, // 4
    1.5f, // 5
    1.5f, // 6
    1.5f, // 7
    0.75f, // 8
    2.0f  // 9
};