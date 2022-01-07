#pragma once

#include "../Common.h"

namespace Colors
{
    glm::vec3 HSVtoRGB(float H, float S, float V)
    {
        float s = S / 100.0f;
        float v = V / 100.0f;
        float C = s * v;
        float X = C * (1.0f - abs(fmod(H / 60.0f, 2) - 1.0f));
        float m = v - C;
        float r, g, b;
        if (H >= 0 && H < 60)
            r = C, g = X, b = 0;
        else if (H >= 60 && H < 120)
            r = X, g = C, b = 0;
        else if (H >= 120 && H < 180)
            r = 0, g = C, b = X;
        else if (H >= 180 && H < 240)
            r = 0, g = X, b = C;
        else if (H >= 240 && H < 300)
            r = X, g = 0, b = C;
        else
            r = C, g = 0, b = X;

        return glm::vec3(r + m, g + m, b + m);
    }
    glm::vec3 HSVtoRGB(glm::vec3 HSV)
    {
        return HSVtoRGB(HSV.x, HSV.y, HSV.z);
    }
}
