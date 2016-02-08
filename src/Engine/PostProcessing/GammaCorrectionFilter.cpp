#include "GammaCorrectionFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostGamma.frag.hpp"

GammaCorrectionFilter::GammaCorrectionFilter() {
    Shader* vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = Resources().CreateShader(POSTGAMMA_FRAG, POSTGAMMA_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
    
    mBrightness = 2.2f;
}

GammaCorrectionFilter::~GammaCorrectionFilter() {
    Resources().FreeShaderProgram(mShaderProgram);
}

ShaderProgram* GammaCorrectionFilter::GetShaderProgram() const {
	return mShaderProgram;
}

void GammaCorrectionFilter::SetUniforms() {
	glUniform1f(mShaderProgram->GetUniformLocation("brightness"), mBrightness);
}

void GammaCorrectionFilter::SetBrightness(float brightness) {
    mBrightness = brightness;
}
