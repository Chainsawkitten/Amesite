#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Geometry {
    class Square;
    class Plane;
}
class Scene;
class Entity;
class Shader;
class ShaderProgram;

/// Holds the frame buffers used for deferred rendering.
class DeferredLighting {
	public:
		/// The different buffer types.
		enum TEXTURE_TYPE {
			DIFFUSE, ///< Diffuse texture
			NORMAL, ///< Normals
			SPECULAR, ///< Specular
            GLOW, ///< Glow
			NUM_TEXTURES ///< Total number of textures (excluding depth buffer)
		};
        
		/// Create new render target.
		/**
		 * @param size Size of the context.
		 */
		DeferredLighting(const glm::vec2& size);
        
		/// Destructor
		/**
		 * Free allocated resources.
		 */
		~DeferredLighting();
        
        /// Set as render target.
        void SetTarget();
        
        /// Reset render target.
        /**
         * Needs to be called before rendering to the screen.
         */
        void ResetTarget();
        
		/// Render the content of diffuse, position and normal textures.
		/**
		 * @param screenSize Size of the screen in pixels.
		 */
		void ShowTextures(const glm::vec2& screenSize);
        
        /// Render the lighting in the scene.
		/**
         * @param scene Scene to get the lights from.
		 * @param camera Camera to use.
		 * @param screenSize Size of the screen in pixels.
		 */
		void Render(Scene& scene, Entity* camera, const glm::vec2& screenSize);
        
	private:
        static void AttachTexture(GLuint texture, unsigned int width, unsigned int height, GLenum attachment, GLint internalFormat);
        void BindForReading();
        void BindForTexReading();
        void SetReadBuffer(TEXTURE_TYPE textureType);
        
		GLuint mTextures[NUM_TEXTURES];
        
		GLuint mFrameBufferObject;
		GLuint mDepthHandle;
        
		glm::vec2 mSize;
        
        // Lights.
        Geometry::Square* mSquare;
        ShaderProgram* mShaderProgram;
        Shader* mVertexShader;
        Shader* mFragmentShader;
        
        // Store light uniform locations so we don't have to get them every frame.
        static const unsigned int mLightCount = 32U;
        struct {
            GLint position;
            GLint intensities;
            GLint attenuation;
            GLint ambientCoefficient;
            GLint coneAngle;
            GLint direction;
        } mLightUniforms[mLightCount];
        
        // Fake lights.
        Geometry::Plane* mPlane;
        ShaderProgram* mFakeShaderProgram;
        Shader* mFakeVertexShader;
        Shader* mFakeFragmentShader;
};
