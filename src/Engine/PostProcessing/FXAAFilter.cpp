#include "FXAAFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostFXAA.frag.hpp"

FXAAFilter::FXAAFilter() {
    Shader* vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = Resources().CreateShader(POSTFXAA_FRAG, POSTFXAA_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    
    mScreenSizeLocation = mShaderProgram->GetUniformLocation("screenSize");
    
    mBrightness = 2.2f;
    mBrightnessLocation = mShaderProgram->GetUniformLocation("brightness");
}

FXAAFilter::~FXAAFilter() {
    Resources().FreeShaderProgram(mShaderProgram);
}

ShaderProgram* FXAAFilter::GetShaderProgram() const {
    return mShaderProgram;
}

void FXAAFilter::SetUniforms() {
    glUniform2fv(mScreenSizeLocation, 1, &mScreenSize[0]);
    glUniform1f(mBrightnessLocation, mBrightness);
}

void FXAAFilter::SetScreenSize(const glm::vec2& screenSize) {
    mScreenSize = screenSize;
}

void FXAAFilter::SetBrightness(float brightness) {
    mBrightness = brightness;
}
