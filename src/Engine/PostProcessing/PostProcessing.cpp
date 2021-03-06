#include "PostProcessing.hpp"

#include "../RenderTarget.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Geometry/Square.hpp"
#include "Filter.hpp"
#include "../Resources.hpp"

PostProcessing::PostProcessing(const glm::vec2& size) {
    mBuffers[0] = new RenderTarget(size);
    mBuffers[1] = new RenderTarget(size);
    
    mSquare = Resources().CreateSquare();
    
    mWhich = 0;
}

PostProcessing::~PostProcessing() {
    delete mBuffers[0];
    delete mBuffers[1];
    
    Resources().FreeSquare();
}

RenderTarget* PostProcessing::GetRenderTarget() const {
    return mBuffers[mWhich];
}

void PostProcessing::ApplyFilter(Filter* filter) {
    // Always pass depth test.
    glDepthFunc(GL_ALWAYS);
    
    mBuffers[1 - mWhich]->SetTarget();
    
    filter->GetShaderProgram()->Use();
    
    glUniform1i(filter->GetShaderProgram()->GetUniformLocation("tDiffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mBuffers[mWhich]->GetColorTexture());
    
    glUniform1i(filter->GetShaderProgram()->GetUniformLocation("tExtra"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mBuffers[mWhich]->GetExtraTexture());
    
    glUniform1i(filter->GetShaderProgram()->GetUniformLocation("tDepth"), 2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mBuffers[mWhich]->GetDepthTexture());
    
    glBindVertexArray(mSquare->GetVertexArray());
    
    filter->SetUniforms();
    
    glDrawElements(GL_TRIANGLES, mSquare->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    // Reset depth testing to standard value.
    glDepthFunc(GL_LESS);
    
    mWhich = 1 - mWhich;
}

void PostProcessing::Render(bool dither) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    mBuffers[mWhich]->Render(dither);
}
