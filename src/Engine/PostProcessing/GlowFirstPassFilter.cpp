#include "GlowFirstPassFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostGlowFirstPass.frag.hpp"

GlowFirstPassFilter::GlowFirstPassFilter() {
    Shader* vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = Resources().CreateShader(POSTGLOWFIRSTPASS_FRAG, POSTGLOWFIRSTPASS_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
}

GlowFirstPassFilter::~GlowFirstPassFilter() {
    Resources().FreeShaderProgram(mShaderProgram);
}

ShaderProgram* GlowFirstPassFilter::GetShaderProgram() const {
	return mShaderProgram;
}

void GlowFirstPassFilter::SetUniforms() {
	
}
