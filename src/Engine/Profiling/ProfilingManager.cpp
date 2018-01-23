#include "ProfilingManager.hpp"

#include "../Util/Log.hpp"
#include "../Resources.hpp"
#include "../Font/Font.hpp"
#include "../Geometry/Square.hpp"
#include "Query.hpp"
#include <GLFW/glfw3.h>

ProfilingManager::ProfilingManager() : active(false) {
    
}

ProfilingManager::~ProfilingManager() {
    
}

ProfilingManager& ProfilingManager::GetInstance() {
    static ProfilingManager instance;
    return instance;
}

void ProfilingManager::Init() {
    for (int i = 0; i < Type::COUNT; ++i) {
        root[i] = new Result("Root: " + TypeToString((Type)i), nullptr);
        root[i]->parent = nullptr;
    }
    
    font = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", 12.f);
    font->SetColor(glm::vec3(1.f, 1.f, 1.f));
    square = Resources().CreateSquare();
}

void ProfilingManager::Free() {
    for (int i = 0; i < Type::COUNT; ++i) {
        delete root[i];
    }
    
    Resources().FreeFont(font);
    Resources().FreeSquare();
}

void ProfilingManager::BeginFrame() {
    if (!active) {
        Log() << "ProfilingManager::BeginFrame warning: Not active.\n";
        return;
    }

    // Clear previous results.
    for (int i = 0; i < Type::COUNT; ++i) {
        root[i]->children.clear();
        current[i] = root[i];
    }
    frameStart = glfwGetTime();
}

void ProfilingManager::EndFrame() {
    // Resolve and reset queries.
    for (auto& it : queryMap) {
        it.first->duration = it.second->Resolve() / 1000000.0;
        queryPool.push_back(it.second);
    }
    queryMap.clear();
}

bool ProfilingManager::IsActive() const {
    return active;
}

void ProfilingManager::SetActive(bool active) {
    this->active = active;
}

ProfilingManager::Result* ProfilingManager::StartResult(const std::string& name, Type type) {
    assert(active);
    assert(type != COUNT);
    
    current[type]->children.push_back(ProfilingManager::Result(name, current[type]));
    Result* result = &current[type]->children.back();
    current[type] = result;
    
    // Begin query if type is GPU.
    if (type == Type::GPU_TIME) {
        // Find available query.
        Query* query;
        if (queryPool.empty())
            query = new Query();
        else {
            query = queryPool.back();
            queryPool.pop_back();
        }
        queryMap[result] = query;
        query->Begin();
    }
    
    return result;
}

void ProfilingManager::FinishResult(Result* result, Type type) {
    assert(active);
    assert(type != COUNT);
    assert(result == current[type]);
    
    // End query if type is GPU.
    if (type == Type::GPU_TIME)
        queryMap[result]->End();
    
    current[type] = result->parent;
}

void ProfilingManager::LogResults() {
    LogResult(*root[CPU_TIME], 0);
    LogResult(*root[GPU_TIME], 0);
}

void ProfilingManager::DrawResults() {
    float y = 0.f;
    DrawResult(*root[CPU_TIME], 0.f, y);
    DrawResult(*root[GPU_TIME], 0.f, y);
}

void ProfilingManager::LogResult(const Result& result, unsigned int indentation) {
    for (unsigned int i=0; i<indentation; ++i)
        Log() << "  ";
    
    Log() << result.name << " " << (result.duration) << " ms\n";
    
    for (const Result& child : result.children)
        LogResult(child, indentation + 1);
}

void ProfilingManager::DrawResult(Result& result, float x, float& y) {
    if (result.parent != nullptr) {
        std::string resultString = result.name + " " + std::to_string(result.duration) + " ms";
        font->RenderText(resultString.c_str(), glm::vec2(x, y), 2000.f);
        
        square->Render(glm::vec2(x - 100.f, y), glm::vec2(100.f, font->GetHeight()), glm::vec3(0.1f, 0.1f, 0.1f));
        square->Render(glm::vec2(x - 100.f, y), glm::vec2(result.duration / result.parent->duration * 100.f, font->GetHeight()), glm::vec3(1.f, 1.f, 1.f));
        
        y += font->GetHeight();
    }
    
    double otherTime = result.duration;
    for (Result& child : result.children) {
        DrawResult(child, x + 100.f, y);
        otherTime -= child.duration;
    }
    
    if (result.parent != nullptr && !result.children.empty()) {
        Result other("Other", &result);
        other.duration = otherTime;
        DrawResult(other, x + 100.f, y);
    }
}

std::string ProfilingManager::TypeToString(ProfilingManager::Type type) {
    switch (type) {
    case ProfilingManager::CPU_TIME:
        return "CPU time (ms)";
        break;
    case ProfilingManager::GPU_TIME:
        return "GPU time (ms)";
        break;
    default:
        assert(false);
        return "ProfilingWindow::TypeToString warning: No valid type to string";
        break;
    }
}

ProfilingManager::Result::Result(const std::string& name, Result* parent) : name (name) {
    this->parent = parent;
}

ProfilingManager& Profiling() {
    return ProfilingManager::GetInstance();
}
