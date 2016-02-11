#pragma once

#include <fstream>
#include <vector>
#include "Geometry3D.hpp"

/** @ingroup Core
 * @{
 */

namespace Geometry {
	class Model;

	/// A model loaded from our own binary model format.
	class Model : public Geometry::Geometry3D {
	public:
		/// Create new model from binary file.
		/**
		 * @param filename Filename (relative or absolute) to the model file.
		 */
		Model(const char* filename);

		/// Destructor
		virtual ~Model();

		/// Get all the vertices.
		/**
		 * @return Array of vertices
		 */
		Vertex* GetVertices() const;

		/// Get the number of vertices.
		/**
		 * @return The number of vertices
		 */
		unsigned int GetVertexCount() const;

		/// Get all the vertex indices.
		/**
		 * @return Array of vertex indices
		 */
		unsigned int* GetIndices() const;

		/// Get the number of indicies.
		/**
		 * @return The number of vertex indices.
		 */
		unsigned int GetIndexCount() const;

		/// Save model to binary file.
		/**
		 * @param filename Filename (relative or absolute) to the model file.
		 */
		void Save(const char* filename) const;

	protected:
		Model() { }

		Vertex *vertexData = nullptr;
		unsigned int vertexNr = 0;

		unsigned int* indexData = nullptr;
		unsigned int indexNr = 0;
	};
}

/** @} */
