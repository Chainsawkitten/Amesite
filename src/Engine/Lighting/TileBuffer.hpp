#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Geometry {
    class Square;
}
class Shader;
class ShaderProgram;

/// Holds the framebuffer that stores the light indices for each tile.
class TileBuffer {
    public:
        /// Create new tile buffer.
        /**
         * @param screenSize Size of the screen (in pixels).
         * @param lightBuffer Light uniform buffer.
         */
        TileBuffer(const glm::vec2& screenSize, GLuint lightBuffer);
        
        /// Destructor.
        ~TileBuffer();
        
        /// Get the tile buffer texture.
        /**
         * @return The tile buffer texture.
         */
        GLuint GetTexture() const;
        
        /// Determine which lights contribute to each tile.
        /**
         * @param lightCount The number of lights currently in view.
         */
        void Calculate(unsigned int lightCount);
        
    private:
        // Size of teach tile.
        static const unsigned short mTileSize = 8;
        
        // Max amount of lights per tile.
        static const unsigned short mMaxLights = 16;
        
        GLuint mTexture;
        GLuint mFrameBuffer;
        
        Geometry::Square* mSquare;
        
        ShaderProgram* mShaderProgram;
        Shader* mVertexShader;
        Shader* mFragmentShader;
        
        unsigned int mHorizontalTiles;
        unsigned int mVerticalTiles;
        
        GLuint mLightBuffer;
        GLuint mLightBufferIndex;
};
