#pragma once

#include "Model.hpp"

/** @ingroup Core
* @{
*/

namespace Geometry {
    /// A model loaded from an OBJ file.
    class OBJModel : public Model {
    public:
        /// Create new model from OBJ file.
        /**
        * @param filename Filename (relative or absolute) to the OBJ model file.
        */
        OBJModel(const char* filename);

    private:
        struct Face {
            struct Vertex {
                unsigned int positionIndex;
                unsigned int normalIndex;
                unsigned int textureIndex;
            };

            Vertex vertices[3];
        };

        static glm::vec3 ReadVec3(std::ifstream& modelFile);
        static glm::vec2 ReadVec2(std::ifstream& modelFile);
        static Face ReadFace(std::ifstream& modelFile);
        static Face::Vertex ReadVertex(std::ifstream& modelFile);
        static void CalculateTangents(std::vector<Face>& faces, std::vector<glm::vec3>& positions, std::vector<glm::vec2>& textureCoordinates, std::vector<glm::vec3>& tangents);
    };
}

/** @} */
