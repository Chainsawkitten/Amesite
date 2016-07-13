#pragma once

#include <glm/glm.hpp>

class RenderTarget;
class Filter;
namespace Geometry {
    class Square;
}

/// Applies post-processing effects to the rendered image.
class PostProcessing {
    public:
        /// Create new post-processing handler.
        /**
         * @param size Size of the context.
         */
        PostProcessing(const glm::vec2& size);
        
        /// Free allocated resources.
        ~PostProcessing();
        
        /// Get render target.
        /**
         * @return Target to render to´.
         */
        RenderTarget* GetRenderTarget() const;
        
        /// Apply a filter to the render image.
        /**
         * filter %Filter to apply.
         */
        void ApplyFilter(Filter* filter);
        
        /// Render resulting image to screen.
        /**
         * @param dither Whether to use dithering.
         */
        void Render(bool dither = false);
        
    private:
        short int mWhich;
        
        RenderTarget* mBuffers[2];
        
        Geometry::Square* mSquare;
};
