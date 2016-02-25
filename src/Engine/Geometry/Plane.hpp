#pragma once

#include "Geometry3D.hpp"

namespace Geometry {
    /// A plane.
    class Plane : public Geometry3D {
        public:
            /// Create new cube at origo.
            Plane();
    
            /// Destructor
            ~Plane();
    
            /// Get all the vertices.
            /**
            * @return Array of vertices
            */
            Vertex* GetVertices() const;
    
            /// Get the number of vertices.
            /**
            * @return The number of vertices
            */
            unsigned int GetVertexCount() const;
    
            /// Get all the vertex indices.
            /**
            * @return Array of vertex indices
            */
            unsigned int* GetIndices() const;
    
            /// Get the number of indicies.
            /**
            * @return The number of vertex indices.
            */
            unsigned int GetIndexCount() const;
    
        private:
            Vertex *mVertexData = nullptr;
            unsigned int mVertexNr = 0;
    
            unsigned int* mIndexData = nullptr;
            unsigned int mIndexNr = 0;
    };
}
