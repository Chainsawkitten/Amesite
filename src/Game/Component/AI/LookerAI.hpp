#pragma once

#include <Component/SuperComponent.hpp>
#include <Util/Input.hpp>
#include <vector>
#include <glm/glm.hpp>

namespace Component {
    /// %Component providing player control.
    class LookerAI : public SuperComponent {
        public:
            /// Create new %Controller.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            LookerAI(Entity* entity);
            
            /// Destructor.
            ~LookerAI();

            ///How far it will look.
            float mDistance;

            /// Updates the walker AI.
            /**
            * @param The delta time
            */
            void Update(float timeDelta);

            ///Adds a point to the path.
            /**
             * @param The point to add.
             */
            void AddEntity(Entity* entity);

        private:
            //The index of the point we're targeting.
            int mTarget;

            //The points to travel through.
            std::vector<Entity*> mTargets;

    };
}
