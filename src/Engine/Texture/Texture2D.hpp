#pragma once

#include "Texture.hpp"
#include <glm/glm.hpp>

namespace Geometry {
    class Square;
}
class Shader;
class ShaderProgram;

/// A two-dimensional texture.
/**
 * Used to load a 2D texture from an image file.
 */
class Texture2D : public Texture {
    public:
        /// Create new texture from the given image file.
        /**
         * Supported image formats: TGA.
         * @param filename Filename (relative or absolute) of the image file.
         * @param srgb Whether the image is in SRGB space and should be converted to linear space.
         */
        Texture2D(const char* filename, bool srgb = false);
        
        /// Create new texture from given source string.
        /**
         * @param source Source string containing the image file.
         * @param sourceLength Length of the source string.
         * @param srgb Whether the image is in SRGB space and should be converted to linear space.
         */
        Texture2D(const char* source, int sourceLength, bool srgb = false);
        
        /// Destructor
        ~Texture2D();
        
        /// Get OpenGL texture ID.
        /**
         * Used when binding a texture before draw calls.
         * @return The OpenGL texture identifier
         */
        GLuint GetTextureID() const;
        
        /// Get the width of the texture.
        /**
         * @return The width of the texture in texels
         */
        int GetWidth() const;
        
        /// Get the height of the texture.
        /**
         * @return The height of the texture in texels
         */
        int GetHeight() const;
        
        /// Set how coordinates outside 0..1 are handled.
        /**
         * Default: GL_REPEAT
         * @param wrapMode One of GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER.
         */
        void SetWrapping(GLint wrapMode);
        
        /// Render the texture to the screen.
        /**
         * @param position Position on the screen, in pixels.
         * @param size Size in pixels.
         */
        void Render(const glm::vec2& position, const glm::vec2& size) const;
        
    private:
        GLuint mTexID;
        int mWidth, mHeight;
        
        Geometry::Square* mSquare;
        
        Shader* mVertexShader;
        Shader* mFragmentShader;
        ShaderProgram* mShaderProgram;
};
