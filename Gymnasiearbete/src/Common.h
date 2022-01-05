#pragma once

#include <iostream>

#include <net.h>
#include "MessageTypes.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

static float randf() { return rand() / (float)RAND_MAX; }