#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    /// Scenery
    class Scenery : public SuperGameObject {
        public:
            /// Create %Scenery
            /**
             * @param scene Pointer to which Scene %Scenery are contained.
             */
            Scenery(Scene* scene, char* model, char* diffuse = nullptr, char* specular = nullptr, char* glow = nullptr);

            ~Scenery();

            /// Node of %Rock
            Entity* node;

        private:

            Geometry::OBJModel* mModel;

            Entity* mLight;

    };
}
