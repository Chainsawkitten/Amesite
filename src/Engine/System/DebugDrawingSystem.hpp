#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Shader;
class ShaderProgram;

namespace System {
    /// Handles debug geometry.
    class DebugDrawingSystem {
        public:
            /// Creates the debug drawing system.
            DebugDrawingSystem();
            
            /// Destructor.
            ~DebugDrawingSystem();
            
            /// Get currently active debug drawing system.
            /**
             * @return The currently active debug drawing system, or nullptr.
             */
            static DebugDrawingSystem* GetActiveInstance();
            
            /// Add a sphere to the world.
            /**
             * @param position The position of the sphere.
             * @param radius The radius of the sphere.
             * @param color Color of the lines.
             * @param lineWidth The width of the lines used to draw the sphere.
             * @param duration How long the sphere should stay in the world (in seconds).
             * @param depthTesting Whether to enable depth testing.
             */
            void AddSphere(const glm::vec3& position, float radius, const glm::vec3& color, float lineWidth = 1.f, float duration = 0.f, bool depthTesting = true);
            
            /// Update the debug geometry.
            /**
             * @param deltaTime Time since last frame (in seconds).
             */
            void Update(float deltaTime);
            
        private:
            DebugDrawingSystem(const DebugDrawingSystem&) = delete;
            void operator=(const DebugDrawingSystem&) = delete;
            
            static void CreateVertexArray(const glm::vec3* positions, unsigned int positionCount, GLuint& vertexBuffer, GLuint& vertexArray);
            void CreateSphere(glm::vec3*& positions, unsigned int& vertexCount, unsigned int detail);
            
            static DebugDrawingSystem* mActiveInstance;
            
            // Shaders.
            Shader* mVertexShader;
            Shader* mFragmentShader;
            ShaderProgram* mShaderProgram;
            
            struct Sphere {
                glm::vec3 position;
                float radius;
                glm::vec3 color;
                float lineWidth;
                float duration;
                bool depthTesting;
            };
            // Uniform locations.
            GLuint mViewProjectionLocation;
            GLuint mModelLocation;
            GLuint mColorLocation;
            GLuint mSizeLocation;
            
            std::vector<Sphere> mSpheres;
            
            // Geometry.
            GLuint mSphereVertexBuffer;
            GLuint mSphereVertexArray;
            unsigned int mSphereVertexCount;
    };
}
