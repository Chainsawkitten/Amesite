#include "Resources.hpp"

#include "Shader/ShaderProgram.hpp"
#include "Shader/Shader.hpp"
#include "Geometry/Cube.hpp"
#include "Geometry/Square.hpp"
#include "Texture/Texture2D.hpp"

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
            key.computeShader = shader->GetSource();
            break;
        case GL_VERTEX_SHADER:
            key.vertexShader = shader->GetSource();
            break;
        case GL_TESS_CONTROL_SHADER:
            key.tessControlShader = shader->GetSource();
            break;
        case GL_TESS_EVALUATION_SHADER:
            key.tessEvaluationShader = shader->GetSource();
            break;
        case GL_GEOMETRY_SHADER:
            key.geometryShader = shader->GetSource();
            break;
        case GL_FRAGMENT_SHADER:
            key.fragmentShader = shader->GetSource();
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

Geometry::Square* ResourceManager::CreateSquare() {
    if (mSquareCount == 0)
        mSquare = new Geometry::Square();
    
    mSquareCount++;
    return mSquare;
}

void ResourceManager::FreeSquare() {
    mSquareCount--;
    
    if (mSquareCount <= 0)
        delete mSquare;
}

Texture2D* ResourceManager::CreateTexture2D(const char* data, int dataLength, bool srgb) {
    if (mTextures.find(data) == mTextures.end()) {
        mTextures[data].texture = new Texture2D(data, dataLength, srgb);
        mTexturesInverse[mTextures[data].texture] = data;
        mTextures[data].count = 1;
    } else {
        mTextures[data].count++;
    }
    
    return mTextures[data].texture;
}

void ResourceManager::FreeTexture2D(Texture2D* texture) {
    if (texture->IsFromFile()) {
        string filename = mTexturesFromFileInverse[texture];
        
        mTexturesFromFile[filename].count--;
        if (mTexturesFromFile[filename].count <= 0) {
            mTexturesFromFileInverse.erase(texture);
            delete texture;
            mTexturesFromFile.erase(filename);
        }
    } else {
        const char* data = mTexturesInverse[texture];
        
        mTextures[data].count--;
        if (mTextures[data].count <= 0) {
            mTexturesInverse.erase(texture);
            delete texture;
            mTextures.erase(data);
        }
    }
}

Texture2D* ResourceManager::CreateTexture2DFromFile(std::string filename, bool srgb) {
    if (mTexturesFromFile.find(filename) == mTexturesFromFile.end()) {
        mTexturesFromFile[filename].texture = new Texture2D(filename.c_str(), srgb);
        mTexturesFromFileInverse[mTexturesFromFile[filename].texture] = filename;
        mTexturesFromFile[filename].count = 1;
    } else {
        mTexturesFromFile[filename].count++;
    }
    
    return mTexturesFromFile[filename].texture;
}

ResourceManager& Resources() {
    return ResourceManager::GetInstance();
}

