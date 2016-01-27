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
    this->mWindow = window;
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

    // Update joystick axis.
    int axisOneCount = 0;
    mJoystickAxisData[PLAYER_ONE] = glfwGetJoystickAxes(PLAYER_ONE, &axisOneCount);
    //Log() << axisOneCount;

    // Update joystick buttons.
    int buttonOneCount = 0;
    mJoystickButtonPressed[PLAYER_ONE] = glfwGetJoystickButtons(PLAYER_ONE, &buttonOneCount);
    //Log() << buttonOneCount;

    // Update joystick axis.
    int axisTwoCount = 0;
    mJoystickAxisData[PLAYER_TWO] = glfwGetJoystickAxes(PLAYER_TWO, &axisTwoCount);
    //Log() << axisTwoCount;

    // Update joystick buttons.
    int buttonTwoCount = 0;
    mJoystickButtonPressed[PLAYER_ONE] = glfwGetJoystickButtons(PLAYER_TWO, &buttonTwoCount);
    //Log() << buttonTwoCount;

    // Update button states depending on bindings.
    for (int player = 0; player < (PLAYERS - 1); player++) {
        for (int button = 0; button < BUTTONS; button++) {
            double value = 0.0;
            for (auto &key : mBindings[player*BUTTONS + button]) {

                // Switch the different input devices.
                switch (mBindingDevice[player][button]) {
                case KEYBOARD:
                    if (glfwGetKey(mWindow, key) == GLFW_PRESS)
                        value = 1.0;
                case JOYSTICK:
                    // Scalar axis of joystick.
                    if (mJoystickAxis[player][button]) {
                        value = mJoystickAxisData[player][key];
                        // Buttons of joystick.
                    }
                    else {
                        if (mJoystickButtonPressed[player][button] == GLFW_PRESS) {
                            value = 1.0;
                        }
                        else {
                            value = 0.0;
                        }
                    }
                default:
                    value = 0.0;
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
        }
        else {
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

InputHandler* Input() {
    return InputHandler::GetActiveInstance();
}
