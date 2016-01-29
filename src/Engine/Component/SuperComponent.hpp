#pragma once

class Entity;

namespace Component{

    // --- TODO PURE VIRTUAL? --- // <------------------------------------------- !!!! <----------------------------------------------------------- !!!! NO MERGE

    /// Component which all Components generalize.
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
        };

}
