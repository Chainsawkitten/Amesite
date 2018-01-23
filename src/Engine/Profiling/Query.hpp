#pragma once

#include <GL/glew.h>
#include <cstdint>

/// Queries information from the GPU.
class Query {
    public:
        /// Create new query.
        Query();
        
        /// Destructor.
        ~Query();
        
        /// Start query.
        void Begin();
        
        /// Stop query.
        void End();
        
        /// Resolve query result.
        /**
         * @return The result of query.
         */
        std::uint64_t Resolve() const;
        
    private:
        Query(const Query & other) = delete;
        
        bool active;
        
        int queryCount;
        GLuint* queries;
        GLuint64* results;
        GLenum target;
};
