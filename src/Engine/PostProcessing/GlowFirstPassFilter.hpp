#pragma once

#include "Filter.hpp"
#include <glm/glm.hpp>

/// Glow filter (first pass).
/**
 * Blurs the glow buffer along the x-axis.
 */
class GlowFirstPassFilter : public Filter {
    public:
        /// Create new color filter.
        GlowFirstPassFilter();
        
        /// Free allocated resources.
        ~GlowFirstPassFilter();
        
        /// Get shader program.
        /**
         * @return Shader program
         */
        ShaderProgram* GetShaderProgram() const;
        
        /// Set uniforms.
        void SetUniforms();
        
    private:
        ShaderProgram* mShaderProgram;
};
