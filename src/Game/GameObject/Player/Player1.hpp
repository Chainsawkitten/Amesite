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
    /// Player1
    class Player1 : public SuperPlayer {
        public:
            /// Create %Player1
            /**
             * @param scene Pointer to which Scene %Player Enities' are contained.
             */
            Player1(Scene* scene);
           
            ~Player1();

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

            /// Gets health of the player.
            /**
             * @return Health of the player.
             */
            float GetHealth();

            /// Activates the player.
            void Activate();
            /// Deactivates the player.
            void Deactivate();

            /// Rotates player.
            void SetYaw(float yaw);

            /// Sets player1 aimed by mouse of not.
            /**
             * @param mouseAim aim with mouse if set true.
             */
            void SetJoystickAim(bool joystickAim);

        private:
            // Default true
            bool mJoystickAim;

            Geometry::OBJModel* mBodyModel;
            Geometry::OBJModel* mFrontEngineModel;
            Geometry::OBJModel* mBackEngineModel;
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

            // Particles relative to engine.
            Entity* mFrontEngineLeftParticles;

            // Particles relative to engine.
            Entity* mFrontEngineRightParticles;

            // Particles relative to engine.
            Entity* mBackEngineLeftParticles;

            // Particles relative to engine.
            Entity* mBackEngineRightParticles;

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

            void AddEnginePartilces(Entity* entity);

            void mUpdateFunction();

            float mRegainAmount;

            float mCollisionRadius;
    };
}
