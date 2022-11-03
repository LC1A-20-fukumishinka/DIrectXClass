#pragma once
#include "Planet.h"
#include <memory>

bool LoadStageFile(int stage , std::vector<std::shared_ptr<Planet>> &planets);