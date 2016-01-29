#pragma once

#include "SuperComponent.hpp"

class Entity;

#include "../Geometry/Geometry3D.hpp"

namespace Component {
    /// Component providing geometry to Entity.
    class Mesh : public SuperComponent {
        public:
            /// Create new %Mesh.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Mesh(Entity* entity);
            
            /// Destructor.
            ~Mesh();

            /// Geometry data.
            /**
             * Default: nullptr
             */
            Geometry::Geometry3D* geometry;
    };
}
