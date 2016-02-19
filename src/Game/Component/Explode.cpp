#include "Explode.hpp"
#include <Engine/Component/ParticleEmitter.hpp>

using namespace Component;

Explode::Explode(Entity* entity) : SuperComponent(entity) {
    lifeTime = 1.f;
    size = 1.f;
    particleTextureIndex = Component::ParticleEmitter::FIRE;
}

Explode::~Explode() {
}
