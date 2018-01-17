#pragma once

#include <GL/glew.h>
#include <stb_truetype.h>
#include <glm/glm.hpp>

class Shader;
class ShaderProgram;
namespace Geometry {
    class Square;
}

/// %Font used for drawing text.
class Font {
    public:
        /// Create new font.
        /**
         * @param filename Filename to TrueType file.
         * @param height Character height.
         */
        Font(const char* filename, float height);
        
        /// Create new font.
        /**
          * @param source TTF source.
          * @param sourceLength Length of source.
          * @param height Character height.
          */
        Font(const char* source, int sourceLength, float height);
        
        /// Free allocated resources.
        ~Font();
        
        /// Render a string to the screen.
        /**
         * @param text Text to render.
         * @param position Position to render the text at (in pixels).
         * @param wrap Width of text before wrapping.
         * @param screenSize Size of the rendering context in pixels.
         * @param blending Whether to use blending when drawing the text.
         */
        void RenderText(const char* text, const glm::vec2& position, float wrap, glm::vec2 screenSize = glm::vec2(0.f, 0.f), bool blending = true);
        
        /// Get rendering color.
        /**
         * @return  Color to render text as.
         */
        glm::vec3 GetColor() const;
        
        /// Set rendering color.
        /**
         * @param color Color to render text as.
         */
        void SetColor(const glm::vec3& color);
        
        /// Get text width.
        /**
         * @param text Text to get the rendered width of.
         * @return The width of the rendered text.
         */
        float GetWidth(const char* text);
        
        /// Get character height.
        /**
         * @return The character height.
         */
        float GetHeight() const;
        
        /// Get baseline.
        /**
         * @return The baseline in pixels.
         */
        int GetBaseline() const;
        
        /// Get whether the font was created from file.
        /**
         * @return true if the font was loaded from a file, false otherwise.
         */
        bool IsFromFile() const;
        
    private:
        bool mIsFromFile;
        
        GLuint mTexture;
        stbtt_bakedchar mCData[96]; // ASCII 32..126 is 95 glyphs
        
        float mHeight;
        unsigned int mBitmapWidth;
        int mBaseline;
        glm::vec3 mColor;
        
        Geometry::Square* mSquare;
        
        // Shaders
        Shader* mVertexShader;
        Shader* mFragmentShader;
        ShaderProgram* mShaderProgram;
        
        float RenderCharacter(char character, const glm::vec2& position, const glm::vec2& screenSize);
        stbtt_aligned_quad BakedQuad(char character, float& x, float& y);
};
