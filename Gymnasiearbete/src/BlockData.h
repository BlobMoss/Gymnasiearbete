#pragma once

#include "glm/glm.hpp"

#define EMPTY 0
#define SOLID 1

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

const glm::vec3 cubeNormals[6] =
{
    glm::vec3(0.0f, -1.0f,  0.0f),
    glm::vec3(0.0f,  1.0f,  0.0f),
    glm::vec3(1.0f,  0.0f,  0.0f),
    glm::vec3(0.0f,  0.0f,  1.0f),
    glm::vec3(-1.0f,  0.0f,  0.0f),
    glm::vec3(0.0f,  0.0f, -1.0f)
};

const unsigned int cubeIndices[12][9] =
{
    { 2, 1, 1, 3, 2, 1, 4, 3, 1 },
    { 5, 3, 2, 8, 4, 2, 7, 1, 2 },
    { 5, 3, 3, 6, 4, 3, 2, 1, 3 },
    { 2, 2, 4, 6, 3, 4, 7, 4, 4 },
    { 7, 3, 5, 8, 4, 5, 4, 1, 5 },
    { 5, 4, 6, 1, 1, 6, 4, 2, 6 },
    { 1, 4, 1, 2, 1, 1, 4, 3, 1 },
    { 6, 2, 2, 5, 3, 2, 7, 1, 2 },
    { 1, 2, 3, 5, 3, 3, 2, 1, 3 },
    { 3, 1, 4, 2, 2, 4, 7, 4, 4 },
    { 3, 2, 5, 7, 3, 5, 4, 1, 5 },
    { 8, 3, 6, 5, 4, 6, 4, 2, 6 }
};