#pragma once

class Texture2D;

/// Reflective water surface.
class Water {
    public:
        /// Create new water surface.
        Water();
        
        /// Destructor.
        ~Water();
        
    private:
        // Textures.
        Texture2D* mWaterTexture;
        Texture2D* mDudvMap;
        Texture2D* mNormalMap;
};
