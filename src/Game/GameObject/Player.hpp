#pragma once

#include <Engine/GameObject/SuperGameObject.hpp>

#include <glm/glm.hpp>

class Scene;
class Entity;
class Texture2D;

namespace Geometry {
    class OBJModel;
}
namespace Audio {
    class SoundBuffer;
}

namespace GameObject {
    /// A Player
    class Player : public SuperGameObject {
        public:
            /// Create %Player
            /**
             * @param scene Pointer to which Scene %Player Enities' are contained.
             */
            Player(Scene* scene);
           
            ~Player();

            /// Gets position of player
            /**
             * @return Position of the player.
             */
            glm::vec3 GetPosition();

            /// Gets health of the player.
            /**
             * @return Health of the player.
             */
            float GetHealth();

            ///Returns true if the player is active.
            bool Active();
            ///Activates the player.
            void Activate();
            ///Deactivates the player.
            void Deactivate();

            /// Node of the %Player.
            Entity* node;

            /// Body relative to node.
            Entity* body;

            /// Engine relative to body.
            Entity* frontEngineLeft;

            /// Engine relative to body.
            Entity* frontEngineRight;

            /// Engine relative to body.
            Entity* backEngineLeft;

            /// Engine relative to body.
            Entity* backEngineRight;

            /// Light relative to body.
            Entity* light;

            /// Turret relative to body.
            Entity* leftTurret;

            /// Turret relative to body.
            Entity* rightTurret;

            void UpdatePlayerTexture();

            ///How long the player has been respawning
            float mRespawnTimer;

        private:
            //The players current state.
            enum PlayerState {
                LIGHTDAMAGE = 0,
                MEDIUMDAMAGE,
                HEAVYDAMAGE
            };

            Geometry::OBJModel* mShipBody;
            Geometry::OBJModel* mShipFrontEngineRight;
            Geometry::OBJModel* mShipFrontEngineLeft;
            Geometry::OBJModel* mShipBackEngineRight;
            Geometry::OBJModel* mShipBackEngineLeft;
            
            Audio::SoundBuffer* mShootSound;

            bool mActive;

            void AddEnginePartilces(Entity* entity);

            PlayerState state;

            Texture2D* mHealthyTexture;
            Texture2D* mHeavyDamageTexture;
            Texture2D* mMediumDamageTexture;
    };
}
