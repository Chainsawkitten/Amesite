#include "FogFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostFog.frag.hpp"

#include "../Component/Lens.hpp"

FogFilter::FogFilter(const glm::vec3& color) {
    mVertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(POSTFOG_FRAG, POSTFOG_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mColor = color;
    mColorLocation = mShaderProgram->GetUniformLocation("color");
    
    mMatrixLocation = mShaderProgram->GetUniformLocation("inverseProjectionMatrix");
    
    mDensity = 0.01f;
    mDensityLocation = mShaderProgram->GetUniformLocation("density");
}

FogFilter::~FogFilter() {
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
}

void FogFilter::SetColor(const glm::vec3& color) {
    mColor = color;
}

void FogFilter::SetDensity(float density) {
    mDensity = density;
}

void FogFilter::SetCamera(const Component::Lens* lens) {
    mLens = lens;
}

void FogFilter::SetScreenSize(const glm::vec2 &screenSize) {
    mScreenSize = screenSize;
}

ShaderProgram* FogFilter::GetShaderProgram() const {
    return mShaderProgram;
}

void FogFilter::SetUniforms() {
    glUniform3fv(mColorLocation, 1, &mColor[0]);
    glUniformMatrix4fv(mMatrixLocation, 1, GL_FALSE, &glm::inverse(mLens->GetProjection(mScreenSize))[0][0]);
    glUniform1f(mDensityLocation, mDensity);
}
