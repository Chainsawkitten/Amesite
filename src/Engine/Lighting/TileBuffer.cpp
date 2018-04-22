#include "TileBuffer.hpp"

#include "../Resources.hpp"
#include "../Geometry/Square.hpp"
#include "../Shader/Shader.hpp"
#include "../Shader/ShaderProgram.hpp"
#include "Post.vert.hpp"
#include "LightTiles.frag.hpp"

#include "../Util/Log.hpp"

TileBuffer::TileBuffer(const glm::vec2& screenSize, GLuint lightBuffer) {
    mLightBuffer = lightBuffer;
    mSquare = Resources().CreateSquare();
    
    // Create shader program.
    mVertexShader = Resources().CreateShader(POST_VERT, POST_VERT_LENGTH, GL_VERTEX_SHADER);
    mFragmentShader = Resources().CreateShader(LIGHTTILES_FRAG, LIGHTTILES_FRAG_LENGTH, GL_FRAGMENT_SHADER);
    mShaderProgram = Resources().CreateShaderProgram({ mVertexShader, mFragmentShader });
    
    // Get light buffer index.
    mLightBufferIndex = mShaderProgram->GetUniformBlockIndex("light_data");
    
    // Create the FBO
    glGenFramebuffers(1, &mFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBuffer);
    
    // Calculate buffer size.
    unsigned int screenWidth = static_cast<unsigned int>(screenSize.x);
    unsigned int screenHeight = static_cast<unsigned int>(screenSize.y);
    mHorizontalTiles = screenWidth / mTileSize + (screenWidth % mTileSize != 0);
    mVerticalTiles = screenHeight / mTileSize + (screenHeight % mTileSize != 0);
    
    // Create texture.
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8UI, mHorizontalTiles, mVerticalTiles * mMaxLights, 0, GL_RED_INTEGER, GL_UNSIGNED_BYTE, nullptr);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);
    
    // Specify draw buffers.
    GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawBuffer);
    
    // Check if framebuffer created correctly
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log() << "Frame buffer creation failed\n";
    
    // Default framebuffer
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

TileBuffer::~TileBuffer() {
    if (mFrameBuffer != 0)
        glDeleteFramebuffers(1, &mFrameBuffer);
    
    glDeleteTextures(1, &mTexture);
    
    Resources().FreeShaderProgram(mShaderProgram);
    Resources().FreeShader(mVertexShader);
    Resources().FreeShader(mFragmentShader);
    
    Resources().FreeSquare();
}

GLuint TileBuffer::GetTexture() const {
    return mTexture;
}

void TileBuffer::Calculate(unsigned int lightCount) {
    mShaderProgram->Use();
    
    // Set framebuffer.
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFrameBuffer);
    
    // Clear framebuffer.
    glViewport(0, 0, mHorizontalTiles, mVerticalTiles * mMaxLights);
    glClear(GL_COLOR_BUFFER_BIT);
    
    if (lightCount == 0)
        return;
    
    // Set uniforms.
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, mLightBuffer);
    mShaderProgram->BindUniformBlock(mLightBufferIndex, 0);
    glUniform1i(mShaderProgram->GetUniformLocation("lightCount"), lightCount);
    
    glBindVertexArray(mSquare->GetVertexArray());
    
    // Calculate light tiles.
    glDrawElements(GL_TRIANGLES, mSquare->GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
}
