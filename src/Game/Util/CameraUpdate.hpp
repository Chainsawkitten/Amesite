#pragma once

#include <vector>

class Scene;
class Entity;

void UpdateCamera(Entity* camera, const std::vector<Entity*>& players);