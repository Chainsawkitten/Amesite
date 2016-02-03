#include "ParticleSystem.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/ParticleEmitter.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#include "../Resources.hpp"
#include "Particle.vert.hpp"
#include "Particle.geom.hpp"
#include "Particle.frag.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

ParticleSystem::ParticleSystem(ParticleType particleType, int maxParticleCount) {
    mVertexShader = Resources().CreateShader(PARTICLE_VERT, PARTICLE_VERT_LENGTH, GL_VERTEX_SHADER);
    mGeometryShader = Resources().CreateShader(PARTICLE_GEOM, PARTICLE_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    mFragmentShader = Resources().CreateShader(PARTICLE_FRAG, PARTICLE_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mGeometryShader, mFragmentShader });
    
    this->mParticleType = particleType;
    
    this->mMaxParticleCount = maxParticleCount;
    mParticleCount = 0;
    
    BindPointData();
}

ParticleSystem::~ParticleSystem() {
    glDeleteBuffers(1, &mVertexBuffer);
    
    for (Component::ParticleEmitter* emitter : mEmitters) {
        delete emitter;
    }
    
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mGeometryShader);
    Resources().FreeShader(mFragmentShader);
}

unsigned int ParticleSystem::ParticleCount() const {
    return mParticleCount;
}

unsigned int ParticleSystem::MaxParticleCount() const {
    return mMaxParticleCount;
}

void ParticleSystem::AddParticleEmitter(Component::ParticleEmitter* emitter) {
    mEmitters.push_back(emitter);
}

void ParticleSystem::RemoveParticleEmitter() {
	mEmitters.pop_back();
}

void ParticleSystem::EmitParticle(glm::vec3 position) {
    if (mParticleCount < mMaxParticleCount) {
        Particle particle;
        
        particle.worldPos = position;
        particle.life = 0.f;
        particle.lifetime = mParticleType.mMinLifetime + rand() / (RAND_MAX / (mParticleType.mMaxLifetime - mParticleType.mMinLifetime));
        
        if (mParticleType.mUniformScaling) {
            particle.size = mParticleType.mMinSize + (rand() / static_cast<float>(RAND_MAX)) * (mParticleType.mMaxSize - mParticleType.mMinSize);
        } else {
            particle.size.x = mParticleType.mMinSize.x + rand() / (RAND_MAX / (mParticleType.mMaxSize.x - mParticleType.mMinSize.x));
            particle.size.y = mParticleType.mMinSize.y + rand() / (RAND_MAX / (mParticleType.mMaxSize.y - mParticleType.mMinSize.y));
        }
        
        particle.velocity.x = mParticleType.mMinVelocity.x + rand() / (RAND_MAX / (mParticleType.mMaxVelocity.x - mParticleType.mMinVelocity.x));
        particle.velocity.y = mParticleType.mMinVelocity.y + rand() / (RAND_MAX / (mParticleType.mMaxVelocity.y - mParticleType.mMinVelocity.y));
        particle.velocity.z = mParticleType.mMinVelocity.z + rand() / (RAND_MAX / (mParticleType.mMaxVelocity.z - mParticleType.mMinVelocity.z));
        
        mParticles.push_back(particle);
        
        mParticleCount++;
    }
}

void ParticleSystem::Update(double time, Entity* follow) {
    if (!this->mParticles.empty()) {
        for (std::vector<int>::size_type i = 0; i != mParticles.size(); i++) {
            mParticles[i].life += static_cast<float>(time);
            
            if (mParticles[i].life >= mParticles[i].lifetime) {
                mParticles.erase(mParticles.begin() + i);
                mParticleCount--;
                i--;
            }
        }
    }
    
    for (Component::ParticleEmitter* emitter : mEmitters) {
        emitter->timeToNext -= time;
        while (emitter->timeToNext < 0.0) {
            emitter->timeToNext += emitter->minEmitTime + ((double)rand() / RAND_MAX) * (emitter->maxEmitTime - emitter->minEmitTime);
            EmitParticle(emitter);
        }
    }
    
    if (mParticleCount > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, mParticleCount * sizeof(ParticleSystem::Particle), &this->mParticles[0]);
    }
}

void ParticleSystem::EmitParticle(Component::ParticleEmitter* emitter) {
    glm::vec3 position;
    if (emitter->emitterType == Component::ParticleEmitter::CUBOID) {
        position.x = emitter->origin.x - emitter->size.x / 2.f + rand() / (RAND_MAX / emitter->size.x);
        position.y = emitter->origin.y - emitter->size.y / 2.f + rand() / (RAND_MAX / emitter->size.y);
        position.z = emitter->origin.z - emitter->size.z / 2.f + rand() / (RAND_MAX / emitter->size.z);
    }
    else if (emitter->emitterType == Component::ParticleEmitter::POINT) {
        position = emitter->origin;
    }
    if (emitter->relative) {
        position += emitter->follow->GetComponent<Component::Transform>()->position;
    }
    EmitParticle(position);
}

void ParticleSystem::Render(Entity* camera, const glm::vec2& screenSize) {
    // Don't write to depth buffer.
    GLboolean depthWriting;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthWriting);
    glDepthMask(GL_FALSE);
    
    // Blending
    GLboolean blending;
    glGetBooleanv(GL_BLEND, &blending);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    mShaderProgram->Use();
    
    glUniform1i(mShaderProgram->GetUniformLocation("baseImage"), 0);
    
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, mParticleType.texture->GetTextureID());
    
    glBindVertexArray(mVertexAttribute);
    
    // Base image texture
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, mParticleType.texture->GetTextureID());
    
    // Send the matrices to the shader.
    glm::mat4 view = camera->GetComponent<Component::Transform>()->GetOrientation() * glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->position);
    glm::vec3 up(glm::inverse(camera->GetComponent<Component::Transform>()->GetOrientation())* glm::vec4(0, 1, 0, 1));
    camera->GetComponent<Component::Transform>()->position;

    glUniform3fv(mShaderProgram->GetUniformLocation("cameraPosition"), 1, &camera->GetComponent<Component::Transform>()->position[0]);
    glUniform3fv(mShaderProgram->GetUniformLocation("cameraUp"), 1, &camera->GetComponent<Component::Transform>()->position[0]);
    glUniformMatrix4fv(mShaderProgram->GetUniformLocation("viewProjectionMatrix"), 1, GL_FALSE, &(camera->GetComponent<Component::Lens>()->GetProjection(screenSize) * view)[0][0]);
    
    float alpha[3] = { mParticleType.mStartAlpha, mParticleType.mMidAlpha, mParticleType.mEndAlpha };
    glUniform1fv(mShaderProgram->GetUniformLocation("alpha"), 3, alpha);
    
    glUniform3fv(mShaderProgram->GetUniformLocation("color"), 1, &mParticleType.mColor[0]);
    
    // Draw the triangles
    glDrawArrays(GL_POINTS, 0, ParticleCount());
    
    // Reset state values we've changed.
    glDepthMask(depthWriting);
    if (!blending)
        glDisable(GL_BLEND);
}

void ParticleSystem::BindPointData() {
    // Vertex buffer
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mMaxParticleCount * sizeof(ParticleSystem::Particle), NULL, GL_DYNAMIC_DRAW);
    
    // Define vertex data layout
    glGenVertexArrays(1, &mVertexAttribute);
    glBindVertexArray(mVertexAttribute);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(sizeof(float) * 3));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(sizeof(float) * 5));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(sizeof(float) * 6));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(sizeof(float) * 7));
    
    glBindVertexArray(0);
}
