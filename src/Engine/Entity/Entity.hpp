#pragma once

namespace Component {
    class Transform;
    class Lens;
    class Mesh;
}

class Entity {
        public:
            Entity();
            ~Entity();

            void CreateLens();

            void CreateTransform();

            void CreateMesh();

            Component::Lens* mLens;
            Component::Transform* mTransform;
            Component::Mesh* mMesh;
};