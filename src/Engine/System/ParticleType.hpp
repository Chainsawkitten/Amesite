#pragma once

#include <glm/glm.hpp>
#include "../Texture/Texture.hpp"

/// Defines a kind of particle.
struct ParticleType {
	/// Texture to apply to the particle.
	Texture* texture = nullptr;
    
	/// The minimum initial velocity in all directions.
	glm::vec3 mMinVelocity;
    
	/// The maximum initial velocity in all directions.
	glm::vec3 mMaxVelocity;
    
	/// The minimum lifetime of the particle (in seconds).
	float mMinLifetime = 0.f;
    
	/// The maximum lifetime of the particle (in seconds).
	float mMaxLifetime = 0.f;
    
	/// The minimum size of the particle.
	glm::vec2 mMinSize = glm::vec2(1.f, 1.f);
    
	/// The maximum size of the particle.
	glm::vec2 mMaxSize = glm::vec2(1.f, 1.f);
    
	/// Whether to scale all axes individually or uniformly.
	bool mUniformScaling;
    
	/// Alpha at the beginning of the particle's life.
	float mStartAlpha = 0.f;
    
	/// Alpha in the middle of the particle's life.
	float mMidAlpha = 1.f;
    
	/// Alpha at the end of the particle's life.
	float mEndAlpha = 0.f;
    
	/// Blend color.
	glm::vec3 mColor = glm::vec3(1.f, 1.f, 1.f);
};
