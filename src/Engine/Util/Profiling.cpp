#include "Profiling.hpp"

#include "Log.hpp"
#include "../Resources.hpp"
#include "../Font/Font.hpp"
#include <GLFW/glfw3.h>

using namespace std;

Profiling::Result Profiling::first("", nullptr);
Profiling::Result* Profiling::current = nullptr;
Font* Profiling::font = nullptr;

Profiling::Profiling(const std::string& name) {
    if (Profiling::current == nullptr) {
        first.name = name;
        first.parent = nullptr;
        current = &first;
        mResult = &first;
    } else {
        current->children.push_back(Result(name, current));
        mResult = &current->children.back();
        current = mResult;
    }
    
    mStart = glfwGetTime();
}

Profiling::~Profiling() {
    mResult->duration = glfwGetTime() - mStart;
    if (current == mResult)
        current = mResult->parent;
}

void Profiling::Init() {
    font = Resources().CreateFontFromFile("Resources/ABeeZee.ttf", 12.f);
    font->SetColor(glm::vec3(1.f, 1.f, 1.f));
}

void Profiling::Free() {
    BeginFrame();
    Resources().FreeFont(font);
}

void Profiling::BeginFrame() {
    // Clear previous results.
    first.children.clear();
    first.name = "";
    first.duration = 0.0;
    current = nullptr;
}

void Profiling::LogResults() {
    LogResult(first, 0);
}

void Profiling::DrawResults() {
    float y = 0.f;
    DrawResult(first, 0.f, y);
}

void Profiling::LogResult(const Result& result, unsigned int indentation) {
    for (unsigned int i=0; i<indentation; ++i)
        Log() << "  ";
    
    Log() << result.name << " " << (result.duration * 1000.0) << " ms\n";
    
    for (const Result& child : result.children)
        LogResult(child, indentation + 1);
}

void Profiling::DrawResult(const Result& result, float x, float& y) {
    string resultString = result.name + " " + to_string(result.duration * 1000.0) + " ms";
    font->RenderText(resultString.c_str(), glm::vec2(x, y), 2000.f);
    y += font->GetHeight();
    
    for (const Result& child : result.children)
        DrawResult(child, x + 10.f, y);
}

Profiling::Result::Result(const std::string& name, Result* parent) {
    this->name = name;
    this->parent = parent;
}
