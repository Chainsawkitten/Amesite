#include "ShaderProgram.hpp"
#include "Shader.hpp"

ShaderProgram::ShaderProgram(std::initializer_list<const Shader*> shaders) {
    mShaderProgram = glCreateProgram();
    
    for (auto shader : shaders)
        glAttachShader(mShaderProgram, shader->GetShaderID());
    
    glLinkProgram(mShaderProgram);
    
    for (auto shader : shaders)
        glDetachShader(mShaderProgram, shader->GetShaderID());
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(mShaderProgram);
}

void ShaderProgram::Use() const {
    glUseProgram(mShaderProgram);
}

GLint ShaderProgram::GetAttributeLocation(const char* name) const {
    return glGetAttribLocation(mShaderProgram, name);
}

GLint ShaderProgram::GetUniformLocation(const char* name) const {
    return glGetUniformLocation(mShaderProgram, name);
}

GLuint ShaderProgram::GetUniformBlockIndex(const char* name) const {
    return glGetUniformBlockIndex(mShaderProgram, name);
}
