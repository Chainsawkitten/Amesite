#pragma once

#include <glm/glm.hpp>
#include <functional>

class Texture2D;
class Font;

/// An option in a menu.
class MenuOption {
    public:
        Texture2D* prerenderedText;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec2 scale;
        
        std::function<void()> callback;
        
        MenuOption(Font* font, const char* text, const glm::vec3& position, const glm::vec3& rotation, float height);
        ~MenuOption();
        
        glm::mat4 GetModelMatrix() const;
        
        void EmptyCallback() const;
};
