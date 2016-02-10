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
            
            /// Diffuse texture.
            Texture2D* diffuse;
            
            /// Specular texture.
            Texture2D* specular;
    };
}
