#include "ColorFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostColor.frag.hpp"

ColorFilter::ColorFilter(const glm::vec3& color) {
    Shader* vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = Resources().CreateShader(POSTCOLOR_FRAG, POSTCOLOR_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    
    mColor = color;
    mColorLocation = mShaderProgram->GetUniformLocation("color");
}

ColorFilter::~ColorFilter() {
    Resources().FreeShaderProgram(mShaderProgram);
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
