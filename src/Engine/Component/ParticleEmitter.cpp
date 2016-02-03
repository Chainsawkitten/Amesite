#include "ParticleEmitter.hpp"

using namespace Component;

ParticleEmitter::ParticleEmitter(Entity* entity) : SuperComponent(entity) {
	lifetime = 0.0;
}
