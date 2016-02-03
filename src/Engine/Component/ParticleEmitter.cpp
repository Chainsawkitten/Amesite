#include "ParticleEmitter.hpp"
#include "../System/ParticleSystem.hpp"
#include "../Entity/Entity.hpp"
#include "Transform.hpp"

using namespace Component;

ParticleEmitter::ParticleEmitter(Entity* entity) : SuperComponent(entity) {
	lifetime = 0.0;
}
