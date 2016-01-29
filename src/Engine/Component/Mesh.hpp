#pragma once

#include "SuperComponent.hpp"

class Entity;

#include "../Geometry/Geometry3D.hpp"

namespace Component {
    /// Component handling a Geometry3D.
    class Mesh : public SuperComponent {
        public:
            /// Create new %Mesh.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Mesh(Entity* entity);
            
            /// Destructor.
            ~Mesh();

            /// Geometry3D.
            /**
             * Default: nullptr
             */
            Geometry::Geometry3D* geometry;
    };
}
