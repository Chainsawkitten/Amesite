#include "ParticleRenderSystem.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <vector>
#include "../Resources.hpp"
#include "../Texture/Texture.hpp"

#include "../Scene/Scene.hpp"
#include "../Entity/Entity.hpp"
#include "../Component/Transform.hpp"
#include "../Component/Physics.hpp"
#include "../Component/ParticleEmitter.hpp"
#include "../Component/Lens.hpp"

#include "../Shader/ShaderProgram.hpp"
#include "../Shader/Shader.hpp"


#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

using namespace System;

ParticleRenderSystem::ParticleRenderSystem()
{
	mParticleVertShader = Resources().CreateShader(PARTICLE_VERT, PARTICLE_VERT_LENGTH, GL_VERTEX_SHADER);
	mParticleGeomShader = Resources().CreateShader(PARTICLE_GEOM, PARTICLE_GEOM_LENGTH, GL_GEOMETRY_SHADER);
	mParticleFragShader = Resources().CreateShader(PARTICLE_FRAG, PARTICLE_FRAG_LENGTH, GL_FRAGMENT_SHADER);
	mParticleShaderProgram = Resources().CreateShaderProgram({ mParticleVertShader, mParticleGeomShader, mParticleFragShader });


    // Vertex buffer
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Particle()->MaxParticleCount() *sizeof(ParticleSystem::Particle), NULL, GL_DYNAMIC_DRAW);

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


ParticleRenderSystem::~ParticleRenderSystem()
{
	Resources().FreeShaderProgram(mParticleShaderProgram);
	Resources().FreeShader(mParticleVertShader);
	Resources().FreeShader(mParticleGeomShader);
	Resources().FreeShader(mParticleFragShader);

    glDeleteBuffers(1, &mVertexBuffer);
}

void ParticleRenderSystem::Render(Scene & scene, Entity* camera)
{
	if (Particle()->ParticleCount() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		// Return vector by value or no?
		glBufferSubData(GL_ARRAY_BUFFER, 0, Particle()->ParticleCount() *sizeof(ParticleSystem::Particle), scene.GetVector<ParticleSystem::Particle>());
	}

    // Don't write to depth buffer.
    GLboolean depthWriting;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthWriting);
    glDepthMask(GL_FALSE);

    // Blending
    GLboolean blending;
    glGetBooleanv(GL_BLEND, &blending);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	std::vector<Component::ParticleEmitter*> emitters = scene.GetAll<Component::ParticleEmitter>();
	Component::ParticleEmitter* emitter = emitters[0]; 
	//Todo: send all textures at once and which which to render using a defined type in GLSL.
	mParticleShaderProgram->Use();

	glUniform1i(mParticleShaderProgram->GetUniformLocation("baseImage"), 0);
	
    glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, emitter->particleType.texture->GetTextureID());

    glBindVertexArray(mVertexAttribute);

    // Base image texture
    glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, emitter->particleType.texture->GetTextureID());

    // Send the matrices to the shader.
    glm::mat4 view = camera->GetComponent<Component::Transform>()->GetOrientation() * glm::translate(glm::mat4(), -camera->GetComponent<Component::Transform>()->position);
    glm::vec3 up(glm::inverse(camera->GetComponent<Component::Transform>()->GetOrientation())* glm::vec4(0, 1, 0, 1));
    camera->GetComponent<Component::Transform>()->position;

	// Ugly hardcoded resolution.
    glUniform3fv(mParticleShaderProgram->GetUniformLocation("cameraPosition"), 1, &camera->GetComponent<Component::Transform>()->position[0]);
    glUniform3fv(mParticleShaderProgram->GetUniformLocation("cameraUp"), 1, &camera->GetComponent<Component::Transform>()->position[0]);
	glUniformMatrix4fv(mParticleShaderProgram->GetUniformLocation("viewProjectionMatrix"), 1, GL_FALSE, &(camera->GetComponent<Component::Lens>()->GetProjection(glm::vec2(800.f, 600.f)) * view)[0][0]);

	float alpha[3] = { emitter->particleType.startAlpha, emitter->particleType.midAlpha, emitter->particleType.endAlpha };
    glUniform1fv(mParticleShaderProgram->GetUniformLocation("alpha"), 3, alpha);

	glUniform3fv(mParticleShaderProgram->GetUniformLocation("color"), 1, &emitter->particleType.color[0]);

    // Draw the triangles
    glDrawArrays(GL_POINTS, 0, Particle()->ParticleCount());

    // Reset state values we've changed.
    glDepthMask(depthWriting);
    if (!blending)
        glDisable(GL_BLEND);
}
