#include "PostProcessing.hpp"

#include "../RenderTarget.hpp"
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

void PostProcessing::Render() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    mBuffers[mWhich]->Render();
}
