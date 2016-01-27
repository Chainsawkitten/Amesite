#pragma once

#include "../Geometry/Geometry3D.hpp"

namespace Component {
    class Mesh
    {
        public:
            Mesh();
            
            ~Mesh();

            Geometry::Geometry3D* geometry;
    };
}
