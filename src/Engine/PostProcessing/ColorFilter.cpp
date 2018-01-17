#include "ColorFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostColor.frag.hpp"

ColorFilter::ColorFilter(const glm::vec3& color) {
    mVertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(POSTCOLOR_FRAG, POSTCOLOR_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    mColor = color;
    mColorLocation = mShaderProgram->GetUniformLocation("color");
}

ColorFilter::~ColorFilter() {
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
}

ShaderProgram* ColorFilter::GetShaderProgram() const {
    return mShaderProgram;
}

void ColorFilter::SetUniforms() {
    glUniform3fv(mColorLocation, 1, &mColor[0]);
}

void ColorFilter::SetColor(const glm::vec3& color) {
    mColor = color;
}
