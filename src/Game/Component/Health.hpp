#pragma once

#include <Component/SuperComponent.hpp>

namespace Component {
    /// %Component containing health.
    class Health : public SuperComponent {
        public:
            /// Create new health component.
            /**
             * @param entity Pointer to which Entity this %Component corresponds.
             */
            Health(Entity* entity);

            /// Destructor.
            ~Health();

            /// Health
            /**
             * Default: 100.f
             */
            float health;

            /// Max health
            /**
             * Default: 100.f
             */
            float maxHealth;

            /// Toughness, percent of damage taken.
            /**
             * Default: 1.f
             */
            float toughness;
            
            /// Regen cooldown, time until entity will regain health (in seconds).
            /**
             * Default: 1.f
             */
            float maxCooldown;

            /// Regen amount, amount of health regenerated each secound (hp/sec).
            /**
             * Default: 1.f
             */
            float regenAmount;

            /// Cooldown, time left until regaining health again. If cooldown < 0.f entity regain health;
            /**
             * Default: 0.f
             */
            float cooldown;

            /// Faction, ID of the faction this health belongs to (players, enemies).
            /**
             * Default: 0
             */
            int faction;

            /// Clear Entity from scene if health is zero.
            /**
             * Default: true
             */
            bool removeOnLowHealth;

            /// Is greater than -1 if it was recently damaged
            /**
             * -1 = not damaged.
             * 0 = damaged last frame.
             * 1 = damaged this frame.
             */
            int damaged;

    };
}
