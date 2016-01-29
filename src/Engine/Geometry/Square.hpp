#pragma once

#include "Geometry2D.hpp"

class Shader;
class ShaderProgram;

namespace Geometry {
    /// A renderable 2D square.
    class Square : public Geometry2D {
        public:
            /// Create new square.
            Square();
            
            /// Destructor.
            ~Square();
            
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
            
            /// Render the square with a solid color.
            /**
             * @param position Position on the screen in pixels.
             * @param size Size in pixels.
             * @param color Color.
             * @param screenSize Size of the screen in pixels.
             */
            void Render(const glm::vec2& position, const glm::vec2& size, const glm::vec3& color, const glm::vec2& screenSize) const;
            
        private:
            Vertex *mVertexData = nullptr;
            unsigned int mVertexNr = 0;
            
            unsigned int* mIndexData = nullptr;
            unsigned int mIndexNr = 0;
            
            // Shaders
            Shader* mVertexShader;
            Shader* mFragmentShader;
            ShaderProgram* mShaderProgram;
    };
}

/** @} */
