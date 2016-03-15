#pragma once

#include <GameObject/SuperGameObject.hpp>

class Scene;
class Entity;
namespace Geometry {
    class OBJModel;
}

namespace GameObject {
    class SuperPlayer;
    
    /// A circle that indicates player revival.
    class ReviveCircle : public SuperGameObject {
        public:
            /// Create revive circle.
            /**
             * @param scene Pointer to which Scene %ReviveCircle Enities' are contained.
             * @param player Player to which teh circle corresponds.
             */
            ReviveCircle(Scene* scene, SuperPlayer* player);
           
            /// Destructor.
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
