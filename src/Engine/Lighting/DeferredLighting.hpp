#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Geometry {
    class Square;
}
class Scene;
class Entity;
class RenderTarget;
class Shader;
class ShaderProgram;
class TileBuffer;

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
         * @param renderTarget Target to render the lit scene to.
		 * @param camera Camera to use.
		 * @param screenSize Size of the screen in pixels.
		 * @param showLightVolumes Render light volumes as spheres.
		 */
		void Render(Scene& scene, RenderTarget* renderTarget, Entity* camera, const glm::vec2& screenSize, bool showLightVolumes = false);
        
        /// Get the tile buffer.
        /**
         * @return The tile buffer.
         */
        TileBuffer* GetTileBuffer() const;
        
	private:
        static void AttachTexture(GLuint texture, unsigned int width, unsigned int height, GLenum attachment, GLint internalFormat);
        
        unsigned int UpdateLightBuffer(Scene& scene, const glm::mat4& view, const glm::mat4& projection, bool showLightVolumes);
        void BindForReading();
        void BindForTexReading();
        void SetReadBuffer(TEXTURE_TYPE textureType);
        
		GLuint mTextures[NUM_TEXTURES];
        
		GLuint mFrameBufferObject;
		GLuint mDepthHandle;
        
		glm::vec2 mSize;
        
        Geometry::Square* mSquare;
        
        ShaderProgram* mShaderProgram;
        Shader* mVertexShader;
        Shader* mFragmentShader;
        
        static const unsigned int mLightCount = 128U;
        struct Light {
            glm::vec4 position;
            glm::vec3 intensities;
            float attenuation;
            glm::vec3 direction;
            float ambientCoefficient;
            float coneAngle;
            float distance;
            float padding1;
            float padding2;
        } mLights[mLightCount];
        GLuint mLightBuffer;
        GLuint mLightBufferIndex;
        
        TileBuffer* mTileBuffer;
};
