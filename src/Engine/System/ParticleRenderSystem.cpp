#include "ParticleRenderSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "../Resources.hpp"
#include "../Texture/Texture.hpp"
#include "../Texture/Texture2D.hpp"

#include "Particle.vert.hpp"
#include "Particle.geom.hpp"
#include "Particle.frag.hpp"

#include "../Scene/Scene.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Lens.hpp"

#include "../Shader/ShaderProgram.hpp"
#include "../Shader/Shader.hpp"


#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

using namespace System;

ParticleRenderSystem::ParticleRenderSystem() {
    mParticleVertShader = Resources().CreateShader(PARTICLE_VERT, PARTICLE_VERT_LENGTH, GL_VERTEX_SHADER);
    mParticleGeomShader = Resources().CreateShader(PARTICLE_GEOM, PARTICLE_GEOM_LENGTH, GL_GEOMETRY_SHADER);
    mParticleFragShader = Resources().CreateShader(PARTICLE_FRAG, PARTICLE_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mParticleShaderProgram = Resources().CreateShaderProgram({ mParticleVertShader, mParticleGeomShader, mParticleFragShader });
    
    // When textures are added to the Atlas the numRows needs to be updated.
    mTextureAtlasNumRows = 4.f;
    
    mTextureAtlas = Resources().CreateTexture2DFromFile("Resources/ParticleAtlasOne.png");
    
    // Vertex buffer
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, Particle().MaxParticleCount() * sizeof(ParticleSystem::Particle), NULL, GL_DYNAMIC_DRAW);
    
    // Define vertex data layout
    glGenVertexArrays(1, &mVertexAttribute);
    glBindVertexArray(mVertexAttribute);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glEnableVertexAttribArray(7);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(sizeof(float) * 3));
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(sizeof(float) * 5));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(sizeof(float) * 6));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(sizeof(float) * 7));
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(sizeof(float) * 10));
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(sizeof(float) * 13));
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleSystem::Particle), BUFFER_OFFSET(sizeof(float) * 16));
    
    glBindVertexArray(0);
}

ParticleRenderSystem::~ParticleRenderSystem() {
    Resources().FreeShaderProgram(mParticleShaderProgram);
    Resources().FreeShader(mParticleVertShader);
    Resources().FreeShader(mParticleGeomShader);
    Resources().FreeShader(mParticleFragShader);
    Resources().FreeTexture2D(mTextureAtlas);
    
    glDeleteBuffers(1, &mVertexBuffer);
}

void ParticleRenderSystem::UpdateBuffer(Scene& scene) {
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    
    // Orphan buffer.
    // See https://www.khronos.org/opengl/wiki/Buffer_Object_Streaming
    glBufferData(GL_ARRAY_BUFFER, Particle().MaxParticleCount() * sizeof(ParticleSystem::Particle), NULL, GL_DYNAMIC_DRAW);
    
    // Transfer particle data.
    glBufferSubData(GL_ARRAY_BUFFER, 0, scene.GetParticleCount() * sizeof(ParticleSystem::Particle), scene.GetParticles());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleRenderSystem::Render(Scene& scene, Entity* camera, const glm::vec2& screenSize, const glm::vec4& clippingPlane) {
    if (scene.GetParticleCount() > 0) {
        // Don't write to depth buffer.
        GLboolean depthWriting;
        glGetBooleanv(GL_DEPTH_WRITEMASK, &depthWriting);
        glDepthMask(GL_FALSE);
        
        // Blending
        glEnablei(GL_BLEND, 0);
        glEnablei(GL_BLEND, 1);
        glBlendFunci(0, GL_SRC_ALPHA, GL_ONE);
        glBlendFunci(1, GL_SRC_ALPHA, GL_ONE);
        
        mParticleShaderProgram->Use();
        
        glBindVertexArray(mVertexAttribute);
        
        glUniform1i(mParticleShaderProgram->GetUniformLocation("baseImage"), 0);
        
        // Base image texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mTextureAtlas->GetTextureID());
        
        // Send the matrices to the shader.
        glm::mat4 view = camera->GetComponent<Component::Transform>()->GetWorldCameraOrientation() * glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->GetWorldPosition());
        glm::vec3 up(glm::inverse(camera->GetComponent<Component::Transform>()->GetWorldCameraOrientation())* glm::vec4(0, 1, 0, 1));
        
        glUniform3fv(mParticleShaderProgram->GetUniformLocation("cameraPosition"), 1, &camera->GetComponent<Component::Transform>()->GetWorldPosition()[0]);
        glUniform3fv(mParticleShaderProgram->GetUniformLocation("cameraUp"), 1, &up[0]);
        glUniformMatrix4fv(mParticleShaderProgram->GetUniformLocation("viewProjectionMatrix"), 1, GL_FALSE, &(camera->GetComponent<Component::Lens>()->GetProjection(screenSize) * view)[0][0]);
        glUniform1fv(mParticleShaderProgram->GetUniformLocation("textureAtlasRows"), 1, &mTextureAtlasNumRows);
        
        // Clipping plane.
        glUniform4fv(mParticleShaderProgram->GetUniformLocation("clippingPlane"), 1, &clippingPlane[0]);
        
        // Draw the triangles
        glDrawArrays(GL_POINTS, 0, scene.GetParticleCount());
        
        // Reset state values we've changed.
        glDepthMask(depthWriting);
        glDisablei(GL_BLEND, 0);
        glDisablei(GL_BLEND, 1);
        
        glUseProgram(0);
        glBindVertexArray(0);
    }
}
