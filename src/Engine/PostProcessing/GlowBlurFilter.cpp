#include "GlowBlurFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostGlowBlur.frag.hpp"

GlowBlurFilter::GlowBlurFilter() {
    Shader* vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = Resources().CreateShader(POSTGLOWBLUR_FRAG, POSTGLOWBLUR_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    
    mScreenSizeLocation = mShaderProgram->GetUniformLocation("screenSize");
    
    mHorizontal = false;
    mHorizontalLocation = mShaderProgram->GetUniformLocation("horizontal");
}

GlowBlurFilter::~GlowBlurFilter() {
    Resources().FreeShaderProgram(mShaderProgram);
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
