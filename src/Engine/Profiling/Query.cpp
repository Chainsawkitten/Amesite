#include "Query.hpp"

#include <GL/glew.h>
#include "../Util/Log.hpp"

Query::Query() : active(false) {
    target = GL_TIMESTAMP;
    
    queryCount = 2;
    
    queries = new GLuint[queryCount];
    results = new GLuint64[queryCount];
    glGenQueries(queryCount, queries);
}

Query::~Query() {
    glDeleteQueries(queryCount, queries);
    delete[] queries;
    delete[] results;
}

void Query::Begin() {
    if (active) {
        Log() << "Query::Begin warning: Already active.\n";
        return;
    }
    
    glQueryCounter(queries[0], target);
    
    active = true;
}

void Query::End() {
    if (!active) {
        Log() << "Query::End warning: Not active.\n";
        return;
    }
    
    glQueryCounter(queries[1], target);
    
    active = false;
}

std::uint64_t Query::Resolve() const {
    if (active) {
        Log() << "Query::Resolve warning: Can't resolve query while active.\n";
        return 0;
    }
    
    for (int i = 0; i < queryCount; ++i) {
        GLuint64 available = GL_FALSE;
        while (available == GL_FALSE)
            glGetQueryObjectui64v(queries[i], GL_QUERY_RESULT_AVAILABLE, &available);
        glGetQueryObjectui64v(queries[i], GL_QUERY_RESULT, &results[i]);
    }
    
    return results[1] - results[0];
}
