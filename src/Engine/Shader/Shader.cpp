#include "Shader.hpp"
#include <vector>
#include "../Util/Log.hpp"

Shader::Shader(const char* source, int sourceLength, GLenum shaderType) {
    mShaderType = shaderType;
    mSource = source;

    mShader = glCreateShader(shaderType);
    GLint length = sourceLength;
    glShaderSource(mShader, 1, &source, &length);
    glCompileShader(mShader);
    
    // Check for shader compilation errors.
    GLint isCompiled = 0;
    glGetShaderiv(mShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &maxLength);
    
        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(mShader, maxLength, &maxLength, &infoLog[0]);
        
        Log() << std::string(infoLog.begin(), infoLog.end());
    }
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

const char* Shader::GetSource() const {
    return mSource;
}
