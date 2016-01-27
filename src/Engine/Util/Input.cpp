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

InputHandler* InputHandler::activeInstance = nullptr;

InputHandler::InputHandler(GLFWwindow* window) {
	this->window = window;
	inputMap[window] = this;

	// Init mouse state.
	for (int i = 0; i<3; i++) {
		mouseState[i] = false;
		mouseStateLast[i] = false;
	}
	glfwSetScrollCallback(window, scrollCallback);
	lastScroll = 0.0;
	scroll = 0.0;

    // Init button states.
    for (int player = 0; player < PLAYERS; player++) {
        for (int button = 0; button < BUTTONS; button++) {
            buttonReleased[player][button] = true;
            buttonTriggered[player][button] = false;
            buttonValue[player][button] = 0.0;
            joystickAxis[player][button] = false;
        }
    }

    bindings = new std::vector<int>[PLAYERS*BUTTONS];

	glfwSetCharCallback(window, characterCallback);
	text = "";
	tempText = "";
}

InputHandler::~InputHandler() {
    delete[] bindings;
}

InputHandler* InputHandler::GetActiveInstance() {
	return activeInstance;
}

void InputHandler::SetActive() {
	activeInstance = this;
}

void InputHandler::Update() {
	// Update mouse.
	for (int i = 0; i<3; i++) {
		mouseStateLast[i] = mouseState[i];
		mouseState[i] = (glfwGetMouseButton(window, i) == GLFW_PRESS);
	}
	lastScroll = scroll;
	scroll = 0.0;

	glfwGetCursorPos(window, &cursorX, &cursorY);

    // Update joystick axis.
    int axisOneCount = 0;
    joystickAxisData[PLAYER_ONE] = glfwGetJoystickAxes(PLAYER_ONE, &axisOneCount);
    //Log() << axisOneCount;

    // Update joystick buttons.
    int buttonOneCount = 0;
    joystickButtonPressed[PLAYER_ONE] = glfwGetJoystickButtons(PLAYER_ONE, &buttonOneCount);
    //Log() << buttonOneCount;

    // Update joystick axis.
    int axisTwoCount = 0;
    joystickAxisData[PLAYER_TWO] = glfwGetJoystickAxes(PLAYER_TWO, &axisTwoCount);
    //Log() << axisTwoCount;

    // Update joystick buttons.
    int buttonTwoCount = 0;
    joystickButtonPressed[PLAYER_ONE] = glfwGetJoystickButtons(PLAYER_TWO, &buttonTwoCount);
    //Log() << buttonTwoCount;

	// Update button states depending on bindings.
    for (int player = 0; player < (PLAYERS-1); player++) {
        for (int button = 0; button < BUTTONS; button++) {
            double value = 0.0;
            for (auto &key : bindings[player*BUTTONS + button]) {

                // Switch the different input devices.
                switch (bindingDevice[player][button]) {
                case KEYBOARD:
                    if (glfwGetKey(window, key) == GLFW_PRESS)
                        value = 1.0;
                case JOYSTICK:
                    // Scalar axis of joystick.
                    if (joystickAxis[player][button]) {
                        value = joystickAxisData[player][key];
                    // Buttons of joystick.
                    } else {
                        if (joystickButtonPressed[player][button] == GLFW_PRESS) {
                            value = 1.0;
                        } else {
                            value = 0.0;
                        }
                    }
                default:
                    value = 0.0;
                }

            }
            buttonTriggered[player][button] = (buttonValue[player][button] == 1.0) && (value == 1.0);
            buttonReleased[player][button] = (buttonValue[player][button] == 0.0) && (value == 0.0);
            buttonValue[player][button] = value;           
        }
    }
    // Update the 'Anyone' input section
    for (int button = 0; button < BUTTONS; button++) {
        buttonTriggered[ANYONE][button] = buttonTriggered[PLAYER_ONE][button] || buttonTriggered[PLAYER_TWO][button];
        buttonReleased[ANYONE][button] = buttonReleased[PLAYER_ONE][button] || buttonReleased[PLAYER_TWO][button];
        if (buttonValue[PLAYER_ONE][button] == 1.0 || buttonValue[PLAYER_ONE][button] == 1.0) {
            buttonValue[ANYONE][button] = 1.0;
        }
        else {
            buttonValue[ANYONE][button] = 0.0;
        }
    }

	// Update text.
	text = tempText;
	tempText = "";
}

bool InputHandler::MousePressed(int button) const {
	return mouseState[button] && !mouseStateLast[button];
}

bool InputHandler::MouseDown(int button) const {
	return mouseState[button];
}

bool InputHandler::MouseReleased(int button) const {
	return !mouseState[button] && mouseStateLast[button];
}

bool InputHandler::ScrollUp() const {
	return lastScroll > 0.0;
}

bool InputHandler::ScrollDown() const {
	return lastScroll < 0.0;
}

double InputHandler::CursorX() const {
	return cursorX;
}

double InputHandler::CursorY() const {
	return cursorY;
}

void InputHandler::CenterCursor() {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glfwSetCursorPos(window, width / 2, height / 2);

	cursorX = static_cast<double>(width / 2);
	cursorY = static_cast<double>(height / 2);
}
void InputHandler::AssignJoystick(Button button, bool axis, int index, Player player) {
    bindings[BUTTONS*player + button].push_back(index);
    joystickAxis[player][button]= axis;
    bindingDevice[player][button] = JOYSTICK;
}

double InputHandler::ButtonValue(Button button, Player player) const {
    return buttonValue[player][button];
}

void InputHandler::AssignKeyboard(Button button, int key, Player player) {
    bindings[BUTTONS*player + button].push_back(key);
    bindingDevice[player][button] = KEYBOARD;
}

bool InputHandler::Pressed(Button button, Player player) {
    if (joystickAxis[player][button])
        Log() << "You are checking if a scalar axis is pressed or not - this will almost never be true.";
	return buttonValue[player][button] == 1.0;
}

bool InputHandler::Triggered(Button button, Player player) {
    if (joystickAxis[player][button])
        Log() << "You are checking if a scalar axis is triggered or not - this will almost never be true.";
	return buttonTriggered[player][button];
}

bool InputHandler::Released(Button button, Player player) {
    if (joystickAxis[player][button])
        Log() << "You are checking if a scalar axis is released or not - this is almost always true.";
	return buttonReleased[player][button];
}

const std::string& InputHandler::Text() const {
	return text;
}

void InputHandler::CharacterCallback(unsigned int codePoint) {
	tempText += static_cast<char>(codePoint);
}

void InputHandler::ScrollCallback(double yoffset) {
	scroll += yoffset;
}

InputHandler* Input() {
	return InputHandler::GetActiveInstance();
}
