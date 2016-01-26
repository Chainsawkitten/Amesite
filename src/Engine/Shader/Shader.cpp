#include "Shader.hpp"

Shader::Shader(const char* source, int sourceLength, GLenum shaderType) {
	this->mShaderType = shaderType;

	mShader = glCreateShader(shaderType);
    GLint length = sourceLength;
	glShaderSource(mShader, 1, &source, &length);
	glCompileShader(mShader);
}

Shader::~Shader() {
	glDeleteShader(mShader);
}

GLuint Shader::GetShaderID() const {
	return mShader;
}

GLenum Shader::GetShaderType() const {
	return mShaderType;
}
