#pragma once

#include <glm/glm.hpp>

namespace Audio {
    /// Wrapper for OpenAL listener.
    /**
     * 3D Sounds are played in relation to the listener's position. Does not need to be created as it is automatically created by the SoundSystem.
     */
    class Listener {
        public:
            /// Create new listener.
            Listener();
            
            /// Get position.
            /**
             * Starting position: (0.0, 0.0, 0.0).
             * @return The listener's position in the game world.
             */
            const glm::vec3& GetPosition() const;
    
            /// Set position.
            /**
             * @param position The listener's new position.
             */
            void SetPosition(const glm::vec3& position);
    
            /// Get the direction the listener is facing.
            /**
             * @return The direction in which the listener is currently facing.
             */
            const glm::vec3& GetForward() const;
    
            /// Get the listener's up-vector.
            /**
             * @return The listener's up-vector.
             */
            const glm::vec3& GetUp() const;
    
            /// Set orientation.
            /**
             * @param forward The direction the listener is facing.
             * @param up The listener's up-vector.
             */
            void SetOrientation(const glm::vec3& forward, const glm::vec3& up);
            
            /// Get listener gain.
            /**
             * @return %Listener gain (0.0 - 1.0).
             */
            float GetGain() const;
            
            /// Set listener gain.
            /**
             * @param gain %Listener gain (0.0 - 1.0).
             */
            void SetGain(float gain);
    
        private:
            glm::vec3 mPosition;
            glm::vec3 mForward;
            glm::vec3 mUp;
            
            float mGain;
    };
}
