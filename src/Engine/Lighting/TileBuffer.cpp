#include "TileBuffer.hpp"

#include "../Util/Log.hpp"

TileBuffer::TileBuffer(const glm::vec2& screenSize) {
    // Create the FBO
    glGenFramebuffers(1, &mFrameBufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObject);
    
    // Calculate buffer size.
    unsigned int screenWidth = static_cast<unsigned int>(screenSize.x);
    unsigned int screenHeight = static_cast<unsigned int>(screenSize.y);
    unsigned int horizontalTiles = screenWidth / mTileSize + (screenWidth % mTileSize != 0);
    unsigned int verticalTiles = screenHeight / mTileSize + (screenHeight % mTileSize != 0);
    unsigned int tileCount = horizontalTiles * verticalTiles;
    
    // Create texture.
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, tileCount, mMaxLights, 0, GL_R, GL_UNSIGNED_BYTE, nullptr);
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
    if (mFrameBufferObject != 0)
        glDeleteFramebuffers(1, &mFrameBufferObject);
    
    glDeleteTextures(1, &mTexture);
}
