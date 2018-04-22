#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Geometry {
    class Square;
}
class Shader;
class ShaderProgram;

/// Holds the framebuffer that stores the light indices for each tile.
/**
 * x is tile index.
 * y is light number.
 */
class TileBuffer {
    public:
        /// Create new tile buffer.
        /**
         * @param screenSize Size of the screen (in pixels).
         */
        TileBuffer(const glm::vec2& screenSize);
        
        /// Destructor.
        ~TileBuffer();
        
    private:
        // Size of teach tile.
        static const unsigned short mTileSize = 8;
        
        // Max amount of lights per tile.
        static const unsigned short mMaxLights = 16;
        
        GLuint mTexture;
        GLuint mFrameBufferObject;
        
        Geometry::Square* mSquare;
        
        ShaderProgram* mShaderProgram;
        Shader* mVertexShader;
        Shader* mFragmentShader;
};
