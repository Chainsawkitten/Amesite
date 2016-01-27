#pragma once

namespace Component {
    class Transform;
    class Lens;
}

class Entity {
        public:
            Entity();
            ~Entity();

            void CreateLens();

            void CreateTransform();

            Component::Lens* mLens;
            Component::Transform* mTransform;
};