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
    for (int i = 0; i<3; i++) {
        mMouseState[i] = false;
        mMouseStateLast[i] = false;
    }
    glfwSetScrollCallback(window, scrollCallback);
    mLastScroll = 0.0;
    mScroll = 0.0;

    // Init button states.
    for (int player = 0; player < PLAYERS; player++) {
        for (int button = 0; button < BUTTONS; button++) {
            mButtonReleased[player][button] = true;
            mButtonTriggered[player][button] = false;
            mButtonValue[player][button] = 0.0;
            mJoystickAxis[player][button] = false;
        }
    }

    // Discover joysticks.
    for (int player = 0; player < PLAYERS - 1; player++) {
        mActiveJoystick[player] = false;
        if (glfwJoystickPresent(player)) {
            Log() << glfwGetJoystickName(player) << " detected! \n";
            mActiveJoystick[player] = true;
        }
    }

    mBindings = new std::vector<int>[PLAYERS*BUTTONS];

    glfwSetCharCallback(window, characterCallback);
    mText = "";
    mTempText = "";
}

InputHandler::~InputHandler() {
    delete[] mBindings;
}

InputHandler* InputHandler::GetActiveInstance() {
    return mActiveInstance;
}

void InputHandler::SetActive() {
    mActiveInstance = this;
}

void InputHandler::Update() {
    // Update mouse.
    for (int i = 0; i<3; i++) {
        mMouseStateLast[i] = mMouseState[i];
        mMouseState[i] = (glfwGetMouseButton(mWindow, i) == GLFW_PRESS);
    }
    mLastScroll = mScroll;
    mScroll = 0.0;

    glfwGetCursorPos(mWindow, &mCursorX, &mCursorY);

    // Joystick counters.
    int axisOneCount = 0;
    int axisTwoCount = 0;
    int buttonOneCount = 0;
    int buttonTwoCount = 0;

    // Update joystick axis and buttons
    for (int player = 0; player < PLAYERS - 1; player++) {
        mJoystickAxisData[player] = glfwGetJoystickAxes(player, &axisOneCount);
        mJoystickButtonPressed[player] = glfwGetJoystickButtons(player, &buttonOneCount);
    }

    // Update button states depending on bindings.
    for (int player = 0; player < (PLAYERS - 1); player++) {
        for (int button = 0; button < BUTTONS; button++) {
            double value = 0.0;
            for (int key : mBindings[player*BUTTONS + button]) {

                // Switch the different input devices.
                switch (mBindingDevice[player][button]) {
                case KEYBOARD:
                    if (glfwGetKey(mWindow, key) == GLFW_PRESS)
                        value = 1.0;
                    break;
                case JOYSTICK:
                    // Scalar axis of joystick.
                    if (mJoystickAxis[player][button]) {
                        value = (abs(mJoystickAxisData[player][key])>mThreshold) ? value = mJoystickAxisData[player][key] : value = 0.0;
                    // Buttons of joystick.
                    } else if (mJoystickButtonPressed[player][mBindings[button][0]] == GLFW_PRESS) {
                        value = 1.0;
                    }
                    break;
                }
            }
            
            mButtonTriggered[player][button] = (mButtonValue[player][button] == 1.0) && (value == 1.0);
            mButtonReleased[player][button] = (mButtonValue[player][button] == 0.0) && (value == 0.0);
            mButtonValue[player][button] = value;
        }
    }
    // Update the 'Anyone' input section
    for (int button = 0; button < BUTTONS; button++) {
        mButtonTriggered[ANYONE][button] = mButtonTriggered[PLAYER_ONE][button] || mButtonTriggered[PLAYER_TWO][button];
        mButtonReleased[ANYONE][button] = mButtonReleased[PLAYER_ONE][button] || mButtonReleased[PLAYER_TWO][button];
        if (mButtonValue[PLAYER_ONE][button] == 1.0 || mButtonValue[PLAYER_ONE][button] == 1.0) {
            mButtonValue[ANYONE][button] = 1.0;
        } else {
            mButtonValue[ANYONE][button] = 0.0;
        }
    }

    // Update text.
    mText = mTempText;
    mTempText = "";
}

bool InputHandler::MousePressed(int button) const {
    return mMouseState[button] && !mMouseStateLast[button];
}

bool InputHandler::MouseDown(int button) const {
    return mMouseState[button];
}

bool InputHandler::MouseReleased(int button) const {
    return !mMouseState[button] && mMouseStateLast[button];
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
void InputHandler::AssignJoystick(Button button, bool axis, int index, Player player) {
    if (!ActiveJoystick(player)) {
        Log() << "Error binding key, Player " << player+1 << " has no joystick connected!\n";
        return;
    }
    mBindings[BUTTONS*player + button].push_back(index);
    mJoystickAxis[player][button] = axis;
    mBindingDevice[player][button] = JOYSTICK;
}

double InputHandler::ButtonValue(Button button, Player player) const {
    return mButtonValue[player][button];
}

void InputHandler::AssignKeyboard(Button button, int key, Player player) {
    mBindings[BUTTONS*player + button].push_back(key);
    mBindingDevice[player][button] = KEYBOARD;
}

bool InputHandler::Pressed(Button button, Player player) {
    if (mJoystickAxis[player][button])
        Log() << "You are checking if a scalar axis is pressed or not - this will almost never be true.";
    return mButtonValue[player][button] == 1.0;
}

bool InputHandler::Triggered(Button button, Player player) {
    if (mJoystickAxis[player][button])
        Log() << "You are checking if a scalar axis is triggered or not - this will almost never be true.";
    return mButtonTriggered[player][button];
}

bool InputHandler::Released(Button button, Player player) {
    if (mJoystickAxis[player][button])
        Log() << "You are checking if a scalar axis is released or not - this is almost always true.";
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

bool InputHandler::ActiveJoystick(Player player) {
    return mActiveJoystick[player];
}

InputHandler* Input() {
    return InputHandler::GetActiveInstance();
}
