#include "ParticleSystem.hpp"

#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include "../Component/ParticleEmitter.hpp"
#include "../Scene/Scene.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#include "../Resources.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
using namespace System;

ParticleSystem::ParticleSystem() { 
    this->mMaxParticleCount = 1000;
    mParticleCount = 0;
    mParticles = new std::vector<Particle>[1];
    
    BindPointData();
}

ParticleSystem::~ParticleSystem() {
}

unsigned int ParticleSystem::ParticleCount(int index) const {
    return mParticleCount[index];
}

unsigned int ParticleSystem::MaxParticleCount(int index) const {
    return mMaxParticleCount[index];
}

void ParticleSystem::EmitParticle(glm::vec3 position, Component::ParticleEmitter* emitter) {
    if (mParticleCount < mMaxParticleCount) {
        Particle particle;
        
        particle.worldPos = position;
        particle.life = 0.f;
        particle.lifetime = emitter->particleType.minLifetime + rand() / (RAND_MAX / (emitter->particleType.maxLifetime - emitter->particleType.minLifetime));
        
        if (emitter->particleType.uniformScaling) {
            particle.size = emitter->particleType.minSize + (rand() / static_cast<float>(RAND_MAX)) * (emitter->particleType.maxSize - emitter->particleType.minSize);
        } else {
            particle.size.x = emitter->particleType.minSize.x + rand() / (RAND_MAX / (emitter->particleType.maxSize.x - emitter->particleType.minSize.x));
            particle.size.y = emitter->particleType.minSize.y + rand() / (RAND_MAX / (emitter->particleType.maxSize.y - emitter->particleType.minSize.y));
        }
        
        particle.velocity.x = emitter->particleType.minVelocity.x + rand() / (RAND_MAX / (emitter->particleType.maxVelocity.x - emitter->particleType.minVelocity.x));
        particle.velocity.y = emitter->particleType.minVelocity.y + rand() / (RAND_MAX / (emitter->particleType.maxVelocity.y - emitter->particleType.minVelocity.y));
        particle.velocity.z = emitter->particleType.minVelocity.z + rand() / (RAND_MAX / (emitter->particleType.maxVelocity.z - emitter->particleType.minVelocity.z));
        
        mParticles.push_back(particle);
        
        mParticleCount++;
    }
}

void ParticleSystem::Update(const Scene& scene, double time) {
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
    
    for (unsigned int i = 0; i < scene.Size<Component::ParticleEmitter>(); i++) {
        Component::ParticleEmitter* emitter = scene.Get<Component::ParticleEmitter>(i);
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
    EmitParticle(position, emitter);
}

void ParticleSystem::Render(Scene* scene, Entity* camera, const glm::vec2& screenSize) {
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
