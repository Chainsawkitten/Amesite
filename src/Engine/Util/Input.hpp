#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <vector>

/// Class that handles input.
/**
 * Rather than checking directly for key presses (like 'W'), keys are assigned to more general buttons (eg. 'Forward' to walk forward).
 * This allows keys to be rebound for different keyboard layouts or player preferences.
 */
class InputHandler {
    public:
        /// Xbox 360 controller scalar axis index
        enum XBOXAxisIndex {
            LEFT_STICK_X = 0, ///< Left stick X axis
            LEFT_STICK_Y, ///< Left stick Y axis
            LEFT_RIGHT_TRIGGER, ///< Left and right trigger value
            RIGHT_STICK_X, ///< Right stick X axis
            RIGHT_STICK_Y, ///< Right stick Y axis
        };

        /// Xbox 360 controller button index
        enum XBOXButtonIndex {
            A = 0, ///< A button
            B, ///< B button
            X, ///< X button
            Y, ///< Y button
            LEFT_BUMPER, ///< Left bumper button
            RIGHT_BUMPER, ///< Right bumper button
            BACK, ///< Back button
            START, ///< Start button
            LEFT_STICK, ///< Left stick click
            RIGHT_STICK, ///< Right stick click
            D_PAD_UP, ///< D-pad up
            D_PAD_RIGHT, ///< D-pad right
            D_PAD_DOWN, ///< D-pad down
            D_PAD_LEFT, ///< D-pad left
        };

        /// Button codes.
        enum Button {
            MOVE_X = 0, ///< Move in X axis
            MOVE_Z, ///< Move in Z axis
            AIM_XZ, ///< Aim in XZ - plane
            SHOOT, ///< Fire bullet
            QUIT, ///< Quit game
            BUTTONS, ///< Total number of inputs - joystick
        };

        /// Player codes
        enum Player {
            PLAYER_ONE = 0, ///< Player one
            PLAYER_TWO, ///< Player two
            ANYONE, ///< Any of the two players
            PLAYERS, ///< Number of players
        };

        /// Input device codes
        enum Device {
            KEYBOARD = 0, ///< Keyboard input
            JOYSTICK, ///< Joystick input
            INPUT_DEVICES, ///< Number of input devices
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
         * @param index of the key in GLFW - See enums for XBOX input in Input.hpp
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
         * @param player - the player to check (0 is anyone, 1 is player1, 2 is player2)
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

        /// Check if the joystick is active/connected
        /**
        * @param player for who to check joystick.
        * @return whether joystick is active or not.
        */
        bool ActiveJoystick(Player player);

    private:
        static InputHandler* mActiveInstance;

        GLFWwindow* mWindow;

        // Mouse states
        bool mMouseState[3];
        bool mMouseStateLast[3];

        double mCursorX, mCursorY;
        double mLastScroll;
        double mScroll;

        std::string mText, mTempText;

        // Binding information, differentiate input devices.
        int mBindingDevice[PLAYERS][BUTTONS];

        // Active joysticks
        bool mActiveJoystick[PLAYERS - 1];

        // Bindings
        std::vector<int>* mBindings;
        double mButtonValue[PLAYERS][BUTTONS];
        bool mButtonReleased[PLAYERS][BUTTONS];
        bool mButtonTriggered[PLAYERS][BUTTONS];

        // Joystick
        bool mJoystickAxis[PLAYERS][BUTTONS];
        const float* mJoystickAxisData[PLAYERS];
        const unsigned char* mJoystickButtonPressed[PLAYERS];
};

/// Get currently active input handler.
/**
 * @return The currently active input handler or nullptr.
 */
InputHandler* Input();
