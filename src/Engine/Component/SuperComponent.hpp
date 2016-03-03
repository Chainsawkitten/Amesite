#pragma once

class Entity;

namespace Component {
    /// %Component which all Components generalize.
    class SuperComponent {
        public:
            /// Create new %SuperComponent.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            SuperComponent(Entity* entity);

            /// Destructor.
            virtual ~SuperComponent();
        
            /// Pointer to which Entity this %Component corresponds.
            /**
             * Default: Must point to an Entity.
             */
            Entity* entity;
            
            /// Kill component, will be removed at end of frame by scene.
            void Kill();
            
            /// Get whether component has been killed.
            /**
             * @return Whether component has been killed.
             */
            bool IsKilled() const;
            
        private:
            bool mKilled;
    };
}
