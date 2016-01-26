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

	keyboardBindings = new std::vector<int>[BUTTONS];

	glfwSetCharCallback(window, characterCallback);
	text = "";
	tempText = "";
}

InputHandler::~InputHandler() {
	delete[] keyboardBindings;
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

	// Update keyboard.
	for (int button = 0; button < BUTTONS; button++) {
		bool pressed = false;
		for (auto &key : keyboardBindings[button]) {
			if (glfwGetKey(window, key) == GLFW_PRESS)
				pressed = true;
		}

		buttonTriggered[button] = !buttonDown[button] && pressed;
		buttonReleased[button] = buttonDown[button] && !pressed;
		buttonDown[button] = pressed;
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

void InputHandler::AssignKeyboard(Button button, int key) {
	keyboardBindings[button].push_back(key);
}

bool InputHandler::Pressed(Button button) {
	return buttonDown[button];
}

bool InputHandler::Triggered(Button button) {
	return buttonTriggered[button];
}

bool InputHandler::Released(Button button) {
	return buttonReleased[button];
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
