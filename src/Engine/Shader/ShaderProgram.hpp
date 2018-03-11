#pragma once

#include <initializer_list>
#include <GL/glew.h>

class Shader;

/// A GLSL shader program.
class ShaderProgram {
	public:
		/// Link shader program from provided shaders.
		/**
		 * Link together shaders into a shader program that can be run on the GPU.
		 *
		 * Sample:
		 * \code{.cpp}
		 * Shader* vertexShader = new Shader(vertexSource, vertexSourceLength, GL_VERTEX_SHADER);
		 * Shader* geometryShader = new Shader(geometrySource, geometrySourceLength, GL_GEOMETRY_SHADER);
		 * Shader* fragmentShader = new Shader(fragmentSource, fragmentSourceLength, GL_FRAGMENT_SHADER);
		 * ShaderProgram* shaderProgram = new ShaderProgram({ vertexShader, geometryShader, fragmentShader });
		 * \endcode
		 *
		 * @param shaders List of shaders to link together.
		 */
		ShaderProgram(std::initializer_list<const Shader*> shaders);

		/// Destructor
		~ShaderProgram();

		/// Use shader program.
		/**
		 * Installs the program object as part of the current rendering state.
		 * @see <a href="https://www.opengl.org/sdk/docs/man4/html/glUseProgram.xhtml">glUseProgram</a>
		 */
		void Use() const;

		/// Get attribute location.
		/**
		 * @param name The name of the attribute variable whose location is to be queried.
		 * @return The location of an attribute variable or -1 if the named attribute variable is not an active attribute in the specified program object or if name starts with the reserved prefix "gl_"
		 */
		GLint GetAttributeLocation(const char* name) const;

		/// Get uniform location.
		/**
		 * @param name The name of the uniform variable whose location is to be queried.
		 * @return The location of a uniform variable or -1 if name does not correspond to an active uniform variable in program, if name starts with the reserved prefix "gl_", or if name is associated with an atomic counter or a named uniform block
		 */
		GLint GetUniformLocation(const char* name) const;
        
        /// Get uniform block index.
		/**
		 * @param name The name of the uniform block whose index is to be queried.
		 * @return The index of the uniform block or GL_INVALID_INDEX.
		 */
		GLuint GetUniformBlockIndex(const char* name) const;
        
        /// Bind a uniform block.
        /**
         * @param uniformBlockIndex The index of the uniform block in the shader program.
         * @param bindingPointIndex The binding point of the uniform buffer.
         */
        void BindUniformBlock(GLuint uniformBlockIndex, GLuint bindingPointIndex);

	private:
		GLuint mShaderProgram;
};
