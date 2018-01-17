#include "GammaCorrectionFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostGamma.frag.hpp"

GammaCorrectionFilter::GammaCorrectionFilter() {
    mVertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(POSTGAMMA_FRAG, POSTGAMMA_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mBrightness = 2.2f;
    mBrightnessLocation = mShaderProgram->GetUniformLocation("brightness");
}

GammaCorrectionFilter::~GammaCorrectionFilter() {
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
}

ShaderProgram* GammaCorrectionFilter::GetShaderProgram() const {
    return mShaderProgram;
}

void GammaCorrectionFilter::SetUniforms() {
    glUniform1f(mBrightnessLocation, 1.f / mBrightness);
}

void GammaCorrectionFilter::SetBrightness(float brightness) {
    mBrightness = brightness;
}
