#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

class Scene;
class Entity;

namespace Component {
    class Transform;
    class Physics;
}

namespace GameObject {
    /// A bullet
    class Bullet : public SuperGameObject {
        public:
            /// Create %Bullet
            Bullet(Scene* scene);
           
            ~Bullet();

            Component::Transform* transform;
            Component::Physics* physics;
    };
}
