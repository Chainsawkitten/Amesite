#pragma once

#include <glm/glm.hpp>

namespace Component{

    /// Component providing transformation
    class Transform {
    public:
        ///Create new transform
        Transform();

        ///Destructor
        ~Transform();

        /// Get model matrix.
        /**
        * @return The object's model matrix, containing translation, scaling and rotation.
        */
        glm::mat4 GetOrientation() const;

        /// Set rotation by an offset.
        /**
        * Rotate the object relative to its current rotation.
        * @param horizontalAngle Relative horizontal rotation (in degrees).
        * @param verticalAngle Relative vertical rotation (in degrees).
        * @param tiltAngle Relative tilt rotation (in degrees).
        */
        void Rotate(float horizontalAngle, float verticalAngle, float tiltAngle);

        /// Move the current position by an offset.
        /**
        * Move the object relative to its current position.
        * @param offset Offset by which to move by.
        */
        void Move(const glm::vec3 &offset);

        /// Move the current position by an offset in x,y,z.
        /**
        * Move the object relative to its current position.
        * @param x offset, y offset, z offset.
        */
        void Move(float x, float y, float z);

        glm::vec3 mPosition;
        glm::vec3 mScale;

        float mHorizontalAngle;
        float mVerticalAngle;
        float mTiltAngle;

    };

}