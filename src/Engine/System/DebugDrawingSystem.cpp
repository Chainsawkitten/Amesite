#include "DebugDrawingSystem.hpp"

#include <glm/gtc/constants.hpp>
#include "../Resources.hpp"
#include "DebugDrawing.vert.hpp"
#include "DebugDrawing.frag.hpp"

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

using namespace System;

DebugDrawingSystem* DebugDrawingSystem::mActiveInstance = nullptr;

DebugDrawingSystem::DebugDrawingSystem() {
    mActiveInstance = this;
    
    mVertexShader = Resources().CreateShader(DEBUGDRAWING_VERT, DEBUGDRAWING_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(DEBUGDRAWING_FRAG, DEBUGDRAWING_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    // Create sphere vertex array.
    glm::vec3* sphere;
    CreateSphere(sphere, mSphereVertexCount, 14);
    CreateVertexArray(sphere, mSphereVertexCount, mSphereVertexBuffer, mSphereVertexArray);
    delete[] sphere;
}

DebugDrawingSystem::~DebugDrawingSystem() {
    mActiveInstance = nullptr;
    
    glDeleteBuffers(1, &mSphereVertexBuffer);
    glDeleteVertexArrays(1, &mSphereVertexArray);
    
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
}

DebugDrawingSystem* DebugDrawingSystem::GetActiveInstance() {
    return mActiveInstance;
}

void DebugDrawingSystem::AddSphere(const glm::vec3& position, float radius, const glm::vec3& color, float lineWidth, float duration, bool depthTesting) {
    Sphere sphere;
    sphere.position = position;
    sphere.radius = radius;
    sphere.color = color;
    sphere.lineWidth = lineWidth;
    sphere.duration = duration;
    sphere.depthTesting = depthTesting;
    mSpheres.push_back(sphere);
}

void DebugDrawingSystem::Update(float deltaTime) {
    // Spheres.
    for (std::size_t i=0; i < mSpheres.size(); ++i) {
        if (mSpheres[i].duration < 0.f) {
            mSpheres[i] = mSpheres[mSpheres.size() - 1];
            mSpheres.pop_back();
            --i;
        } else
            mSpheres[i].duration -= deltaTime;
    }
}

void DebugDrawingSystem::CreateVertexArray(const glm::vec3* positions, unsigned int positionCount, GLuint& vertexBuffer, GLuint& vertexArray) {
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, positionCount * sizeof(glm::vec3), positions, GL_STATIC_DRAW);
    
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), BUFFER_OFFSET(0));
    
    glBindVertexArray(0);
}

// Create UV-sphere with given number of parallel and meridian lines.
void DebugDrawingSystem::CreateSphere(glm::vec3*& positions, unsigned int& vertexCount, unsigned int detail) {
    vertexCount = detail * (4 * detail - 2);
    positions = new glm::vec3[vertexCount];
    
    // Horizontal lines (meridians).
    unsigned int i = 0;
    for (unsigned int m = 1; m < detail; ++m) {
        float meridian = glm::pi<float>() * m / detail;
        for (unsigned int p = 0; p <= detail; ++p) {
            float parallel = 2.0f * glm::pi<float>() * p / detail;
            float angle = glm::pi<float>() * 0.5f - meridian;
            float y = sin(angle);
            float x = cos(angle);
            positions[i++] = glm::vec3(x * cos(parallel), y, x * sin(parallel));
            if (p > 0 && p < detail)
                positions[i++] = glm::vec3(x * cos(parallel), y, x * sin(parallel));
        }
    }
    
    // Vertical lines (parallels).
    for (unsigned int p = 0; p < detail; ++p) {
        float parallel = 2.0f * glm::pi<float>() * p / detail;
        for (unsigned int m = 0; m <= detail; ++m) {
            float meridian = glm::pi<float>() * m / detail;
            float angle = glm::pi<float>() * 0.5f - meridian;
            float y = sin(angle);
            float x = cos(angle);
            positions[i++] = glm::vec3(x * cos(parallel), y, x * sin(parallel));
            if (m > 0 && m < detail)
                positions[i++] = glm::vec3(x * cos(parallel), y, x * sin(parallel));
        }
    }
}
