#include "ParticleRenderSystem.h"



System::ParticleRenderSystem::ParticleRenderSystem()
{
}


System::ParticleRenderSystem::~ParticleRenderSystem()
{
    glDeleteBuffers(1, &mVertexBuffer);
}

void System::ParticleRenderSystem::Render(const Scene & scene)
{
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
