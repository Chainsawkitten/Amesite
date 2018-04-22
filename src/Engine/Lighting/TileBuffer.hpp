#pragma once

#include <glm/glm.hpp>

/// Holds the framebuffer that stores the light indices for each tile.
class TileBuffer {
    public:
        /// Create new tile buffer.
        /**
         * @param screenSize Size of the screen (in pixels).
         */
        TileBuffer(const glm::vec2& screenSize);
        
        /// Destructor.
        ~TileBuffer();
};
