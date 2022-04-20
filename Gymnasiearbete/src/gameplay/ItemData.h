#pragma once

// Blocks:

#define EMPTY 0
#define PLANKS 1
#define SAND 2
#define GRASS 3

// Items:

#define CANNONBALL 17
#define CUTLASS 18
#define FLINTLOCK 19
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
    false, // 18
    false, // 19
    true, // 20
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
    true, // 32
};

const float breakTimes[] =
{
    0.0f,
    1.0f,
    2.5f,
    2.5f
};