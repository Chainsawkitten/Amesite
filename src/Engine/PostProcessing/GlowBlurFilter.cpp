#include "GlowBlurFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostGlowBlur.frag.hpp"

GlowBlurFilter::GlowBlurFilter() {
    mVertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(POSTGLOWBLUR_FRAG, POSTGLOWBLUR_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mScreenSizeLocation = mShaderProgram->GetUniformLocation("screenSize");
    
    mHorizontal = false;
    mHorizontalLocation = mShaderProgram->GetUniformLocation("horizontal");
}

GlowBlurFilter::~GlowBlurFilter() {
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
}

ShaderProgram* GlowBlurFilter::GetShaderProgram() const {
    return mShaderProgram;
}

void GlowBlurFilter::SetUniforms() {
    glUniform2fv(mScreenSizeLocation, 1, &mScreenSize[0]);
    glUniform1f(mHorizontalLocation, mHorizontal);
}

void GlowBlurFilter::SetScreenSize(const glm::vec2& screenSize) {
    mScreenSize = screenSize;
}

void GlowBlurFilter::SetHorizontal(bool horizontal) {
    mHorizontal = horizontal ? 1.f : 0.f;
}
