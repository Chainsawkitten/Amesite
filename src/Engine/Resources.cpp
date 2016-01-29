#include "Resources.hpp"

#include "Shader/ShaderProgram.hpp"
#include "Shader/Shader.hpp"
#include "Geometry/Cube.hpp"

using namespace std;

ResourceManager::ResourceManager() {
    
}

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    
    return instance;
}

Shader* ResourceManager::CreateShader(const char* source, int sourceLength, GLenum shaderType) {
    if (mShaders.find(source) == mShaders.end()) {
        mShaders[source].shader = new Shader(source, sourceLength, shaderType);
        mShadersInverse[mShaders[source].shader] = source;
        mShaders[source].count = 1;
    } else {
        mShaders[source].count++;
    }
    
    return mShaders[source].shader;
}

void ResourceManager::FreeShader(Shader* shader) {
    const char* source = mShadersInverse[shader];
    
    mShaders[source].count--;
    if (mShaders[source].count <= 0) {
        mShadersInverse.erase(shader);
        delete shader;
        mShaders.erase(source);
    }
}

ShaderProgram* ResourceManager::CreateShaderProgram(std::initializer_list<const Shader*> shaders) {
    ShaderProgramKey key;
    
    for (auto shader : shaders) {
        switch (shader->GetShaderType()) {
        case GL_COMPUTE_SHADER:
            key.computeShader = shader;
            break;
        case GL_VERTEX_SHADER:
            key.vertexShader = shader;
            break;
        case GL_TESS_CONTROL_SHADER:
            key.tessControlShader = shader;
            break;
        case GL_TESS_EVALUATION_SHADER:
            key.tessEvaluationShader = shader;
            break;
        case GL_GEOMETRY_SHADER:
            key.geometryShader = shader;
            break;
        case GL_FRAGMENT_SHADER:
            key.fragmentShader = shader;
            break;
        }
    }
    
    if (mShaderPrograms.find(key) == mShaderPrograms.end()) {
        ShaderProgramInstance shaderProgram;
        shaderProgram.shaderProgram = new ShaderProgram(shaders);
        shaderProgram.count = 1;
        mShaderPrograms[key] = shaderProgram;
        mShaderProgramsInverse[shaderProgram.shaderProgram] = key;
    } else {
        mShaderPrograms[key].count++;
    }
    
    return mShaderPrograms[key].shaderProgram;
}

void ResourceManager::FreeShaderProgram(ShaderProgram* shaderProgram) {
    ShaderProgramKey key = mShaderProgramsInverse[shaderProgram];
    mShaderPrograms[key].count--;
    
    if (mShaderPrograms[key].count <= 0) {
        mShaderProgramsInverse.erase(shaderProgram);
        delete shaderProgram;
        mShaderPrograms.erase(key);
    }
}

ResourceManager::ShaderProgramKey::ShaderProgramKey() {
    computeShader = nullptr;
    vertexShader = nullptr;
    tessControlShader = nullptr;
    tessEvaluationShader = nullptr;
    geometryShader = nullptr;
    fragmentShader = nullptr;
}

bool ResourceManager::ShaderProgramKey::operator<(const ShaderProgramKey& other) const {
    if (computeShader < other.computeShader) return true;
    if (computeShader > other.computeShader) return false;
    
    if (vertexShader < other.vertexShader) return true;
    if (vertexShader > other.vertexShader) return false;
    
    if (tessControlShader < other.tessControlShader) return true;
    if (tessControlShader > other.tessControlShader) return false;
    
    if (tessEvaluationShader < other.tessEvaluationShader) return true;
    if (tessEvaluationShader > other.tessEvaluationShader) return false;
    
    if (geometryShader < other.geometryShader) return true;
    if (geometryShader > other.geometryShader) return false;
    
    if (fragmentShader < other.fragmentShader) return true;
    if (fragmentShader > other.fragmentShader) return false;
    
    return false;
}

Geometry::Cube* ResourceManager::CreateCube() {
    if (mCubeCount == 0)
        mCube = new Geometry::Cube();
    
    mCubeCount++;
    return mCube;
}

void ResourceManager::FreeCube() {
    mCubeCount--;
    
    if (mCubeCount <= 0)
        delete mCube;
}

ResourceManager& Resources() {
    return ResourceManager::GetInstance();
}

