#pragma once

#include "../Geometry/Geometry3D.hpp"

namespace Component {
    ///Component providing geometry to entity
    class Mesh
    {
        public:
            Mesh();
            
            ~Mesh();

            ///Geometry data
            Geometry::Geometry3D* geometry;
    };
}
