#pragma once

#include <GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    class SuperPlayer;
    /// A ReviveCircle
    class ReviveCircle : public SuperGameObject {
        public:
            /// Create %ReviveCircle
            /**
             * @param scene Pointer to which Scene %ReviveCircle Enities' are contained.
             */
            ReviveCircle(Scene* scene, SuperPlayer* player);
           
            ~ReviveCircle();

            /// Node of %ReviveCircle
            Entity* node;
            /// Outer ring relative to node
            Entity* outerRing;
            /// Inner circle relative to node
            Entity* innerCircle;

        private:
            Geometry::OBJModel* mRingModel;
            Geometry::OBJModel* mCircleModel;

            SuperPlayer* mPlayer;

            void mUpdateFunction();
    };
}
