#pragma once

#include "SuperComponent.hpp"

class Entity;
class Texture2D;

namespace Component {
    /// %Component providing a material to Entity.
    class Material : public SuperComponent {
        public:
            /// Create new material.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Material(Entity* entity);
            
            /// Destructor.
            ~Material();
            
            /// Set diffuse texture from file.
            /**
             * @param filename Path to the image file.
             */
            void SetDiffuse(const char* filename);
            
            /// Set specular texture from file.
            /**
             * @param filename Path to the image file.
             */
            void SetSpecular(const char* filename);
            
            /// Diffuse texture.
            Texture2D* diffuse;
            
            /// Specular texture.
            Texture2D* specular;
    };
}
