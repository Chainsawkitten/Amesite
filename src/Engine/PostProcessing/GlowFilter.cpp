#include "GlowFilter.hpp"

#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "../Resources.hpp"
#include "Post.vert.hpp"
#include "PostGlow.frag.hpp"

GlowFilter::GlowFilter() {
    Shader* vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = Resources().CreateShader(POSTGLOW_FRAG, POSTGLOW_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    Resources().FreeShader(fragmentShader);
}

GlowFilter::~GlowFilter() {
    Resources().FreeShaderProgram(mShaderProgram);
}

ShaderProgram* GlowFilter::GetShaderProgram() const {
    return mShaderProgram;
}

void GlowFilter::SetUniforms() {
    
}
