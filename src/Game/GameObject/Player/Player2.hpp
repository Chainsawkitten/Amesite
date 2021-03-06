#pragma once

#include "SuperPlayer.hpp"

class Texture2D;

namespace Geometry {
    class OBJModel;
}
namespace Audio {
    class SoundBuffer;
}

namespace GameObject {
    /// Player2
    class Player2 : public SuperPlayer {
        public:
            /// Create %Player2
            /**
             * @param scene Pointer to which Scene %Player Enities' are contained.
             */
            Player2(Scene* scene);
    
            ~Player2();
    
            /// Gets position of player
            /**
             * @return Position of the player.
             */
            glm::vec3 GetPosition();
    
            /// Sets position of player
            /**
             * @param position Position of the player.
             */
            void SetPosition(glm::vec3 position);
    
            /// Gets node Entity
            /**
             * @return Main Entity contaning tranform and physics components.
             */
            Entity* GetNodeEntity();
            
            /// Gets body entity.
            /**
             * This is the entity containing the player's main mesh and is different from the node entity as the animation should not affect collision deteciton.
             * @return The entity containing the player's main mesh.
             */
            Entity* GetBodyEntity();
    
            /// Gets health of the player.
            /**
             * @return Health of the player.
             */
            float GetHealth();

            /// Set device
            /**
             * @param Device to control player
             */
            void SetDevice(InputHandler::Device device);
    
            /// Activates the player.
            void Activate();
            
            /// Deactivates the player.
            void Deactivate();
    
            /// Rotates player.
            void SetYaw(float yaw);
    
        private:
            Geometry::OBJModel* mBodyModel;
            Geometry::OBJModel* mEngineModel;
            Geometry::OBJModel* mPropellerModel;
            Geometry::OBJModel* mTurretBodyModel;
            Geometry::OBJModel* mTurretBarrelModel;
    
            Audio::SoundBuffer* mShootSound;
    
            Texture2D* mHealthyTexture;
            Texture2D* mHeavyDamageTexture;
            Texture2D* mMediumDamageTexture;
            Texture2D* mDeadTexture;
    
            // Node of %Player1.
            Entity* mNode;
    
            // Body relative to node.
            Entity* mBody;
    
            // Engine relative to body.
            Entity* mFrontEngineLeft;
    
            // Engine relative to body.
            Entity* mFrontEngineRight;
    
            // Engine relative to body.
            Entity* mBackEngineLeft;
    
            // Engine relative to body.
            Entity* mBackEngineRight;
    
            // Propeller relative to body.
            Entity* mMidPropeller;
    
            // Particles relative to mid propeller;
            Entity* mMidPropellerParticles[4];
    
            // Propeller relative to engine.
            Entity* mFrontPropellerLeft;
    
            // Propeller relative to engine.
            Entity* mFrontPropellerRight;
    
            // Propeller relative to engine.
            Entity* mBackPropellerLeft;
    
            // Propeller relative to engine.
            Entity* mBackPropellerRight;
    
            // Light relative to body.
            Entity* mLight;
    
            // Turret relative to body.
            Entity* mLeftTurretBody;
    
            // Turret relative to body.
            Entity* mRightTurretBody;
    
            struct Barrel {
                Entity* barrel[2];
                Entity* node;
            };
    
            void CreateBarrel(Barrel* barrel);
    
            // Barrel relative to turret.
            Barrel mLeftTurretBarrel;
    
            // Barrel relative to turret.
            Barrel mRightTurretBarrel;
    
            // SpawnNode relative to barrel.
            Entity* mLeftSpawnNode;
    
            // SpawnNode relative to barrel.
            Entity* mRightSpawnNode;
    
            // Light under the player.
            Entity* mBottomLight;
    
            void AddEngine(Entity* entity, glm::vec3 position, glm::vec3 scale);
    
            void AddMidPropeller(Entity* entity, glm::vec3 position, glm::vec3 scale);
    
            void AddPropeller(Entity* entity, glm::vec3 position, glm::vec3 scale);
    
            void mUpdateFunction();
    
            float mRegainAmount;
    
            float mCollisionRadius;
    };
}
