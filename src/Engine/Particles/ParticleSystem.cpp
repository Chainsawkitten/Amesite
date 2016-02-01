#include "ParticleSystem.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#include "../Resources.hpp"
#include "Particle.vert.hpp"
#include "Particle.geom.hpp"
#include "Particle.frag.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

ParticleSystem::ParticleSystem(ParticleType particleType, int maxParticleCount) {
    vertexShader = Resources().CreateShader(PARTICLE_VERT, PARTICLE_VERT_LENGTH, GL_VERTEX_SHADER);
    geometryShader = Resources().CreateShader(PARTICLE_GEOM, PARTICLE_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    fragmentShader = Resources().CreateShader(PARTICLE_FRAG, PARTICLE_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    shaderProgram = Resources().CreateShaderProgram({ vertexShader, geometryShader, fragmentShader });
    
    this->particleType = particleType;
    
    this->maxParticleCount = maxParticleCount;
    particleCount = 0;
    
    BindPointData();
}

ParticleSystem::~ParticleSystem() {
    glDeleteBuffers(1, &vertexBuffer);
    
    for (ParticleEmitter* emitter : emitters) {
        delete emitter;
    }
    
    Resources().FreeShaderProgram(shaderProgram);
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(geometryShader);
    Resources().FreeShader(fragmentShader);
}

unsigned int ParticleSystem::ParticleCount() const {
    return particleCount;
}

unsigned int ParticleSystem::MaxParticleCount() const {
    return maxParticleCount;
}

void ParticleSystem::AddParticleEmitter(ParticleEmitter* emitter) {
    emitters.push_back(emitter);
}

void ParticleSystem::RemoveParticleEmitter() {
	emitters.pop_back();
}

void ParticleSystem::EmitParticle(glm::vec3 position) {
    if (particleCount < maxParticleCount) {
        Particle particle;
        
        particle.worldPos = position;
        particle.life = 0.f;
        particle.lifetime = particleType.minLifetime + rand() / (RAND_MAX / (particleType.maxLifetime - particleType.minLifetime));
        
        if (particleType.uniformScaling) {
            particle.size = particleType.minSize + (rand() / static_cast<float>(RAND_MAX)) * (particleType.maxSize - particleType.minSize);
        } else {
            particle.size.x = particleType.minSize.x + rand() / (RAND_MAX / (particleType.maxSize.x - particleType.minSize.x));
            particle.size.y = particleType.minSize.y + rand() / (RAND_MAX / (particleType.maxSize.y - particleType.minSize.y));
        }
        
        particle.velocity.x = particleType.minVelocity.x + rand() / (RAND_MAX / (particleType.maxVelocity.x - particleType.minVelocity.x));
        particle.velocity.y = particleType.minVelocity.y + rand() / (RAND_MAX / (particleType.maxVelocity.y - particleType.minVelocity.y));
        particle.velocity.z = particleType.minVelocity.z + rand() / (RAND_MAX / (particleType.maxVelocity.z - particleType.minVelocity.z));
        
        particles.push_back(particle);
        
        particleCount++;
    }
}

void ParticleSystem::Update(double time, Entity* follow) {
    if (!this->particles.empty()) {
        for (std::vector<int>::size_type i = 0; i != particles.size(); i++) {
            particles[i].life += static_cast<float>(time);
            
            if (particles[i].life >= particles[i].lifetime) {
                particles.erase(particles.begin() + i);
                particleCount--;
                i--;
            }
        }
    }
    
    for (ParticleEmitter* emitter : emitters) {
        emitter->Update(time, this, follow);
    }
    
    if (particleCount > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(ParticleSystem::Particle), &this->particles[0]);
    }
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
    
    shaderProgram->Use();
    
    glUniform1i(shaderProgram->GetUniformLocation("baseImage"), 0);
    
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, particleType.texture->GetTextureID());
    
    glBindVertexArray(vertexAttribute);
    
    // Base image texture
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, particleType.texture->GetTextureID());
    
    // Send the matrices to the shader.
    glm::mat4 view = camera->GetComponent<Component::Transform>()->GetOrientation() * glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->position);
    glm::vec3 up(glm::inverse(camera->GetComponent<Component::Transform>()->GetOrientation())* glm::vec4(0, 1, 0, 1));
    camera->GetComponent<Component::Transform>()->position;

    glUniform3fv(shaderProgram->GetUniformLocation("cameraPosition"), 1, &camera->GetComponent<Component::Transform>()->position[0]);
    glUniform3fv(shaderProgram->GetUniformLocation("cameraUp"), 1, &camera->GetComponent<Component::Transform>()->position[0]);
    glUniformMatrix4fv(shaderProgram->GetUniformLocation("viewProjectionMatrix"), 1, GL_FALSE, &(camera->GetComponent<Component::Lens>()->GetProjection(screenSize) * view)[0][0]);
    
    float alpha[3] = { particleType.startAlpha, particleType.midAlpha, particleType.endAlpha };
    glUniform1fv(shaderProgram->GetUniformLocation("alpha"), 3, alpha);
    
    glUniform3fv(shaderProgram->GetUniformLocation("color"), 1, &particleType.color[0]);
    
    // Draw the triangles
    glDrawArrays(GL_POINTS, 0, ParticleCount());
    
    // Reset state values we've changed.
    glDepthMask(depthWriting);
    if (!blending)
        glDisable(GL_BLEND);
}

void ParticleSystem::BindPointData() {
    // Vertex buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, maxParticleCount * sizeof(ParticleSystem::Particle), NULL, GL_DYNAMIC_DRAW);
    
    // Define vertex data layout
    glGenVertexArrays(1, &vertexAttribute);
    glBindVertexArray(vertexAttribute);
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
