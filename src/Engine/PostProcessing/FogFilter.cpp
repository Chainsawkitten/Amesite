#include "FogFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostFog.frag.hpp"

#include "../Component/Lens.hpp"

FogFilter::FogFilter(const glm::vec3& color) {
    Shader* vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = Resources().CreateShader(POSTFOG_FRAG, POSTFOG_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    
    mColor = color;
    mDensity = 0.01f;
}

FogFilter::~FogFilter() {
    Resources().FreeShaderProgram(mShaderProgram);
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
    glUniform3fv(mShaderProgram->GetUniformLocation("color"), 1, &mColor[0]);
    glUniformMatrix4fv(mShaderProgram->GetUniformLocation("inverseProjectionMatrix"), 1, GL_FALSE, &glm::inverse(mLens->GetProjection(mScreenSize))[0][0]);
    glUniform1f(mShaderProgram->GetUniformLocation("density"), mDensity);
}
