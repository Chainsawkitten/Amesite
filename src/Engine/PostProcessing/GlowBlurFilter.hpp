#pragma once

#include "Filter.hpp"
#include <glm/glm.hpp>
#include <GL/glew.h>

class Shader;

/// Glow filter (first pass).
/**
 * Blurs the glow buffer along a specified axis.
 */
class GlowBlurFilter : public Filter {
    public:
        /// Create new color filter.
        GlowBlurFilter();
        
        /// Free allocated resources.
        ~GlowBlurFilter();
        
        /// Get shader program.
        /**
         * @return Shader program
         */
        ShaderProgram* GetShaderProgram() const;
        
        /// Set uniforms.
        void SetUniforms();
        
        /// Set the screen size used when blurring.
        /**
         * @param screenSize Size of the screen in pixels.
         */
        void SetScreenSize(const glm::vec2& screenSize);
        
        /// Set axis to blur.
        /**
         * @param horizontal Whether the blur should be horizontal (otherwise vertical).
         */
        void SetHorizontal(bool horizontal);
        
    private:
        Shader* mVertexShader;
        Shader* mFragmentShader;
        ShaderProgram* mShaderProgram;
        
        glm::vec2 mScreenSize;
        GLint mScreenSizeLocation;
        
        float mHorizontal;
        GLint mHorizontalLocation;
};
