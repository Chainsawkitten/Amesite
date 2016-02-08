#pragma once

#include "Filter.hpp"
#include <glm/glm.hpp>

/// Gamma correction filter.
class GammaCorrectionFilter : public Filter {
    public:
        /// Create new color filter.
        GammaCorrectionFilter();
        
        /// Free allocated resources.
        ~GammaCorrectionFilter();
        
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
