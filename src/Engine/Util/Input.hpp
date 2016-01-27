#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <vector>

/** @ingroup Core
 * @{
 */

/// Class that handles input.
/**
 * Rather than checking directly for key presses (like 'W'), keys are assigned to more general buttons (eg. 'Forward' to walk forward).
 * This allows keys to be rebound for different keyboard layouts or player preferences.
 */
class InputHandler {
    public:
        /// Button codes.
        enum Button {
            MOVE_X = 0, ///< Move in X axis
            MOVE_Z, ///< Move in Z axis
            AIM_X, ///< Aim in X axis
            SHOOT, ///< Fire bullet
            QUIT, ///< Quit game
            BUTTONS, ///< Total number of inputs - joystick
        };
        enum Player {
            PLAYER_ONE = 0,
            PLAYER_TWO,
            ANYONE,
            PLAYERS,
        };
        enum InputDevices {
            KEYBOARD = 0,
            JOYSTICK,
            INPUT_DEVICES,
        };
        
        /// Create new input handler.
        /**
         * @param window %Window to get input for.
         */
        InputHandler(GLFWwindow* window);
        
        /// Destructor.
        ~InputHandler();
        
        /// Get currently active input handler.
        /**
         * @return The currently active input handler or nullptr.
         */
        static InputHandler* GetActiveInstance();
        
        /// Set as currently active input handler.
        void SetActive();
        
        /// Update input state.
        void Update();
        
        /// Get whether a mouse button was pressed.
        /**
         * @param button Mouse button to check. Either GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT or GLFW_MOUSE_BUTTON_MIDDLE.
         * @return Whether button has been pressed since last frame
         */
        bool MousePressed(int button) const;
        
        /// Get whether a mouse button is down.
        /**
         * @param button Mouse button to check. Either GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT or GLFW_MOUSE_BUTTON_MIDDLE.
         * @return Whether button is down
         */
        bool MouseDown(int button) const;
        
        /// Get whether a mouse button was released.
        /**
         * @param button Mouse button to check. Either GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT or GLFW_MOUSE_BUTTON_MIDDLE.
         * @return Whether button has been released since last frame
         */
        bool MouseReleased(int button) const;
        
        /// Get whether user has moved scroll wheel up.
        /**
         * @return Whether user has scrolled up
         */
        bool ScrollUp() const;
        
        /// Get whether user has moved scroll wheel down.
        /**
         * @return Whether user has scrolled down
         */
        bool ScrollDown() const;
        
        /// Get cursor's horizontal position.
        /**
         * @return X-position of the cursor
         */
        double CursorX() const;
        
        /// Get cursor's vertical position.
        /**
         * @return Y-position of the cursor
         */
        double CursorY() const;
        
        /// Centers the cursor to the middle of the window.
        void CenterCursor();
        
        /// Assign a joystick key to a button.
        /**
        * @param button The button to assign a key to.
        * @param axis - is it an axis (==TRUE)? or is it a button (==FALSE).
        * @param index of the key in GLFW - 
        * @param player to check (0 player 1, 1 is player2, 2 is player3)
        */
        void AssignJoystick(Button button, bool axis, int index, Player player);

        /// Gets the value of an axis on joystick
        /**
        * @param button The button to check.
        * @param player to check (0 player 1, 1 is player2, 2 is player3)
        * @return a float with a value between -1 and 1 - representing axis value.
        */
        double ButtonValue(Button button, Player player) const;

        /// Assign a keyboard key to a button.
        /**
         * @param button The button to assign a key to.
         * @param key The <a href="http://www.glfw.org/docs/latest/group__keys.html">keyboard key</a> to assign.
         * @param playerNum - the player to check (0 is anyone, 1 is player1, 2 is player2)
         */
        void AssignKeyboard(Button button, int key, Player player);
        
        /// Gets whether a button is currently down.
        /**
         * @param button The button to check.
         * @param player to check (0 player 1, 1 is player2, 2 is player3)
         * @return Whether the button is down
         */
        bool Pressed(Button button, Player player);
        
        /// Gets whether a button was just pressed.
        /**
         * Checks whether a button was pressed between the last two calls to update().
         * @param button The button to check.
         * @param player to check (0 player 1, 1 is player2, 2 is player3)
         * @return Whether the button was pressed
         */
        bool Triggered(Button button, Player player);
        
        /// Gets whether a button was just released.
        /**
         * Checks whether a button was released between the last two calls to update().
         * @param button The button to check.
         * @param player to check (0 player 1, 1 is player2, 2 is player3)
         * @return Whether the button was released
         */
        bool Released(Button button, Player player);
        
        /// Get text input since last frame.
        /**
         * @return Text input since last frame.
         */
        const std::string& Text() const;
        
        /// GLFW character callback.
        /**
         * @param codePoint Unicode code point.
         */
        void CharacterCallback(unsigned int codePoint);
        
        /// GLFW scrolling callback.
        /**
         * @param yoffset Offset along the Y-axis.
         */
        void ScrollCallback(double yoffset);
        
    private:
        static InputHandler* activeInstance;
        
        GLFWwindow* window;
        
        // Mouse states
        bool mouseState[3];
        bool mouseStateLast[3];
        
        double cursorX, cursorY;
        double lastScroll;
        double scroll;
        
        std::string text, tempText;

        // Binding information, differentiate input devices.
        int bindingDevice[PLAYERS][BUTTONS];

        // Bindings
        std::vector<int>* bindings;
        double buttonValue[PLAYERS][BUTTONS];
        bool buttonReleased[PLAYERS][BUTTONS];
        bool buttonTriggered[PLAYERS][BUTTONS];

        // Joystick
        bool joystickAxis[PLAYERS][BUTTONS];
        const float* joystickAxisData[PLAYERS];
        const unsigned char* joystickButtonPressed[PLAYERS];


};

/// Get currently active input handler.
/**
 * @return The currently active input handler or nullptr.
 */
InputHandler* Input();

/** @} */
