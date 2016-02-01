#pragma once

#include <glm/glm.hpp>
#include "../Texture/Texture.hpp"

/// Defines a kind of particle.
struct ParticleType {
	/// Texture to apply to the particle.
	Texture* texture = nullptr;
    
	/// The minimum initial velocity in all directions.
	glm::vec3 minVelocity;
    
	/// The maximum initial velocity in all directions.
	glm::vec3 maxVelocity;
    
	/// The minimum lifetime of the particle (in seconds).
	float minLifetime = 0.f;
    
	/// The maximum lifetime of the particle (in seconds).
	float maxLifetime = 0.f;
    
	/// The minimum size of the particle.
	glm::vec2 minSize = glm::vec2(1.f, 1.f);
    
	/// The maximum size of the particle.
	glm::vec2 maxSize = glm::vec2(1.f, 1.f);
    
	/// Whether to scale all axes individually or uniformly.
	bool uniformScaling;
    
	/// Alpha at the beginning of the particle's life.
	float startAlpha = 0.f;
    
	/// Alpha in the middle of the particle's life.
	float midAlpha = 1.f;
    
	/// Alpha at the end of the particle's life.
	float endAlpha = 0.f;
    
	/// Blend color.
	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
};
