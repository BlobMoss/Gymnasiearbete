#pragma once

#include "glm/glm.hpp"

#define EMPTY 0
#define PLANKS 1
#define SAND 2
#define GRASS 3

const float breakTimes[] =
{
    0.0f,
    1.0f,
    2.5f,
    2.5f
};

const glm::vec3 cubePositions[8] =
{
    glm::vec3(0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f,  0.5f),
    glm::vec3(-0.5f, -0.5f, -0.5f),
    glm::vec3(0.5f,  0.5f, -0.5f),
    glm::vec3(0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f,  0.5f),
    glm::vec3(-0.5f,  0.5f, -0.5f)
};

const glm::vec2 cubeUvs[4] =
{
    glm::vec2(0.0f, 0.0f),
    glm::vec2(1.0f, 0.0f),
    glm::vec2(1.0f, 1.0f),
    glm::vec2(0.0f, 1.0f)
};

const glm::vec3 cubeNormals[5] =
{
    glm::vec3(0.0f,  1.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(0.0f,  0.0f,  1.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(0.0f,  0.0f, -1.0f)
};

const unsigned int cubeIndices[10][9] =
{
    { 5, 3, 1, 8, 4, 1, 7, 1, 1 },
    { 5, 3, 2, 6, 4, 2, 2, 1, 2 },
    { 2, 2, 3, 6, 3, 3, 7, 4, 3 },
    { 7, 3, 4, 8, 4, 4, 4, 1, 4 },
    { 5, 4, 5, 1, 1, 5, 4, 2, 5 },
    { 6, 2, 1, 5, 3, 1, 7, 1, 1 },
    { 1, 2, 2, 5, 3, 2, 2, 1, 2 },
    { 3, 1, 3, 2, 2, 3, 7, 4, 3 },
    { 3, 2, 4, 7, 3, 4, 4, 1, 4 },
    { 8, 3, 5, 5, 4, 5, 4, 2, 5 }
};