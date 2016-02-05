#include "Log.hpp"
#include "Input.hpp"
#include <map>

std::map<GLFWwindow*, InputHandler*> inputMap;

void characterCallback(GLFWwindow* window, unsigned int codePoint) {
    inputMap[window]->CharacterCallback(codePoint);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    inputMap[window]->ScrollCallback(yoffset);
}

InputHandler* InputHandler::mActiveInstance = nullptr;

InputHandler::InputHandler(GLFWwindow* window) {
    mWindow = window;
    inputMap[window] = this;
    
    // Init mouse state.
    glfwSetScrollCallback(window, scrollCallback);
    mLastScroll = 0.0;
    mScroll = 0.0;
    
    // Init button states.
    for (int player = 0; player < PLAYERS; player++) {
        for (int button = 0; button < BUTTONS; button++) {
            mButtonReleased[player][button] = true;
            mButtonTriggered[player][button] = false;
            mButtonValue[player][button] = 0.0;
        }
    }
    
    // Discover joysticks.
    for (int player = 0; player < PLAYERS - 1; player++) {
        mJoystickActive[player] = false;
        if (glfwJoystickPresent(player)) {
            Log() << glfwGetJoystickName(player) << " detected! \n";
            mJoystickActive[player] = true;
        }
    }
    
    glfwSetCharCallback(window, characterCallback);
    mText = "";
    mTempText = "";
}

InputHandler::~InputHandler() {
    
}

InputHandler* InputHandler::GetActiveInstance() {
    return mActiveInstance;
}

void InputHandler::SetActive() {
    mActiveInstance = this;
}

void InputHandler::Update() {
    // Check if controller was disconnected or reconnected.
    for (int player = 0; player < PLAYERS - 1; player++) {
        if (glfwJoystickPresent(player) == GLFW_FALSE && mJoystickActive[player]) {
            mJoystickActive[player] = false;
            Log() << "Player " << player+1 << " joystick was disconnected! \n";
        } else if (glfwJoystickPresent(player) == GLFW_TRUE && !mJoystickActive[player]) {
            mJoystickActive[player] = true;
            Log() << "Player " << player+1 <<" joystick ("<< glfwGetJoystickName(player) <<") was connected!" << "\n";
        }
    }
    
    mLastScroll = mScroll;
    mScroll = 0.0;
    
    glfwGetCursorPos(mWindow, &mCursorX, &mCursorY);
    
    // Joystick counters.
    int axisCount = 0;
    int buttonCount = 0;
    const float* joystickAxisData[PLAYERS];
    const unsigned char* joystickButtonPressed[PLAYERS];
    
    // Update joystick axis and buttons.
    for (int player = 0; player < PLAYERS - 1 && mJoystickActive[player]; player++) {
        joystickAxisData[player] = glfwGetJoystickAxes(player, &axisCount);
        joystickButtonPressed[player] = glfwGetJoystickButtons(player, &buttonCount);
    }
    
    // Get button states.
    double values[PLAYERS-1][BUTTONS] = { { 0.0 } };
    for (Binding binding : mBindings) {
        double value = 0.0;
        switch (binding.device) {
        case KEYBOARD:
            if (glfwGetKey(mWindow, binding.index) == GLFW_PRESS)
                value = 1.0;
            break;
        case MOUSE:
            if (glfwGetMouseButton(mWindow, binding.index) == GLFW_PRESS)
                value = 1.0;
            break;
        case JOYSTICK:
            if (mJoystickActive[binding.player]) {
                if (binding.axis) {
                    value = (abs(joystickAxisData[binding.player][binding.index]) > mThreshold) ? value = joystickAxisData[binding.player][binding.index] : value = 0.0;
                } else if (joystickButtonPressed[binding.player][binding.index] == GLFW_PRESS) {
                    value = 1.0;
                }
            }
            break;
        }
        
        if (values[binding.player][binding.button] == 0.0)
            values[binding.player][binding.button] = value;
    }
    
    // Update triggered and released.
    for (int player=0; player<PLAYERS-1; player++) {
        for (int button=0; button<BUTTONS; button++) {
            mButtonTriggered[player][button] = (mButtonValue[player][button] == 1.0) && (values[player][button] == 1.0);
            mButtonReleased[player][button] = (mButtonValue[player][button] == 0.0) && (values[player][button] == 0.0);
            mButtonValue[player][button] = values[player][button];
            
            // Update anyone buttons.
            if (mButtonTriggered[player][button])
                mButtonTriggered[ANYONE][button] = true;
            
            if (mButtonTriggered[player][button])
                mButtonTriggered[ANYONE][button] = true;
            
            if (mButtonValue[player][button] != 0.0)
                mButtonValue[ANYONE][button] = mButtonValue[player][button];
        }
    }
    
    // Update text.
    mText = mTempText;
    mTempText = "";
}

bool InputHandler::ScrollUp() const {
    return mLastScroll > 0.0;
}

bool InputHandler::ScrollDown() const {
    return mLastScroll < 0.0;
}

double InputHandler::CursorX() const {
    return mCursorX;
}

double InputHandler::CursorY() const {
    return mCursorY;
}

void InputHandler::CenterCursor() {
    int width, height;
    glfwGetFramebufferSize(mWindow, &width, &height);
    glfwSetCursorPos(mWindow, width / 2, height / 2);
    
    mCursorX = static_cast<double>(width / 2);
    mCursorY = static_cast<double>(height / 2);
}

void InputHandler::AssignButton(Player player, Button button, Device device, int index, bool axis) {
    Binding binding;
    binding.player = player;
    binding.button = button;
    binding.device = device;
    binding.index = index;
    binding.axis = axis;
    
    mBindings.push_back(binding);
}

double InputHandler::ButtonValue(Player player, Button button) const {
    return mButtonValue[player][button];
}

bool InputHandler::Pressed(Player player, Button button) {
    return mButtonValue[player][button] == 1.0;
}

bool InputHandler::Triggered(Player player, Button button) {
    return mButtonTriggered[player][button];
}

bool InputHandler::Released(Player player, Button button) {
    return mButtonReleased[player][button];
}

const std::string& InputHandler::Text() const {
    return mText;
}

void InputHandler::CharacterCallback(unsigned int codePoint) {
    mTempText += static_cast<char>(codePoint);
}

void InputHandler::ScrollCallback(double yoffset) {
    mScroll += yoffset;
}

bool InputHandler::JoystickActive(Player player) {
    return mJoystickActive[player];
}

InputHandler* Input() {
    return InputHandler::GetActiveInstance();
}
