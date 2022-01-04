#pragma once

#include "glm/glm.hpp"

const unsigned int symbolWidth = 8;
const unsigned int symbolHeight = 10;

const glm::vec2 squarePositions[4] = {
     glm::vec2(0.0f       , 0.0f        ),
     glm::vec2(symbolWidth, 0.0f        ),
     glm::vec2(symbolWidth, symbolHeight),
     glm::vec2(0.0f       , symbolHeight)
};

const glm::vec2 squareUvs[4] = {
     glm::vec2(0.0f, 0.0f), 
     glm::vec2(1.0f, 0.0f), 
     glm::vec2(1.0f, 1.0f),
     glm::vec2(0.0f, 1.0f)
};

const unsigned int squareIndices[2][6] = {
    { 1, 1, 2, 2, 3, 3 },
    { 3, 3, 4, 4, 1 ,1 }
};