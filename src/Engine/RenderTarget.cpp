#include "RenderTarget.hpp"
#include "Util/Log.hpp"

#include "Shader/Shader.hpp"
#include "Shader/ShaderProgram.hpp"

#include "Geometry/Square.hpp"

#include "Resources.hpp"
#include "Post.vert.hpp"
#include "PostCopy.frag.hpp"
#include "PostDither.frag.hpp"

RenderTarget::RenderTarget(const glm::vec2 &size) {
    mWidth = static_cast<int>(size.x);
    mHeight = static_cast<int>(size.y);
    
    // Color buffer.
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mColorBuffer);
    glBindTexture(GL_TEXTURE_2D, mColorBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Extra buffer (eg. bloom).
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mExtraBuffer);
    glBindTexture(GL_TEXTURE_2D, mExtraBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Depth buffer.
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &mDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, mDepthBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    // Framebuffer objects
    glGenFramebuffers(1, &mFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mExtraBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer, 0);
    
    // Initialize draw buffers.
    GLenum drawBuffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, drawBuffers);
    
    // Check if framebuffer created correctly
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log() << "Framebuffer creation failed\n";
    
    // Default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    Shader* vertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    Shader* fragmentShader = Resources().CreateShader(POSTCOPY_FRAG, POSTCOPY_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(fragmentShader);
    
    fragmentShader = Resources().CreateShader(POSTDITHER_FRAG, POSTDITHER_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mDitherShaderProgram = Resources().CreateShaderProgram({ vertexShader, fragmentShader });
    Resources().FreeShader(vertexShader);
    
    mSquare = Resources().CreateSquare();
    mDitherTime = 0.f;
}

RenderTarget::~RenderTarget() {
    glDeleteTextures(1, &mDepthBuffer);
    glDeleteTextures(1, &mExtraBuffer);
    glDeleteTextures(1, &mColorBuffer);
    glDeleteFramebuffers(1, &mFrameBuffer);
    
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShaderProgram(mDitherShaderProgram);
    
    Resources().FreeSquare();
}

void RenderTarget::SetTarget() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBuffer);
}

void RenderTarget::SetSource() {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFrameBuffer);
}

glm::vec2 RenderTarget::GetSize() const {
    return glm::vec2(static_cast<float>(mWidth), static_cast<float>(mHeight));
}

GLuint RenderTarget::GetColorTexture() const {
    return mColorBuffer;
}

GLuint RenderTarget::GetExtraTexture() const {
    return mExtraBuffer;
}

GLuint RenderTarget::GetDepthTexture() const {
    return mDepthBuffer;
}

void RenderTarget::Render(bool dither) {
    // Always pass depth test.
    glDepthFunc(GL_ALWAYS);
    
    ShaderProgram* shader = dither ? mDitherShaderProgram : mShaderProgram;
    shader->Use();
    
    glUniform1i(shader->GetUniformLocation("tDiffuse"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mColorBuffer);
    
    glUniform1i(shader->GetUniformLocation("tDepth"), 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mDepthBuffer);
    
    if (dither) {
        glUniform1f(shader->GetUniformLocation("time"), mDitherTime);
        mDitherTime = fmod(mDitherTime + 1.f, 255.f);
    }
    
    glBindVertexArray(mSquare->GetVertexArray());
    
    glDrawElements(GL_TRIANGLES, mSquare->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    
    // Reset depth testing to standard value.
    glDepthFunc(GL_LESS);
}
