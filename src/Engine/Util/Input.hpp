#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
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
            AIM_X, ///< Aim in X axis
            AIM_Z, ///< Aim in Z axis
            SHOOT, ///< Fire bullet
            BOOST, ///< Fire bullet
            SHIELD, ///< Shield
            QUIT, ///< Quit game
            UP, ///< Move up
            DOWN, ///< Move down
            RIGHT, ///< Move right
            LEFT, ///< Move left
            PAUSE, ///< Pause game
            PROFILE, ///< Run profiling
            LOG_PROFILE, ///< Output profiling results to log.
            BUTTONS, ///< Total number of inputs
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
            NONE = 0, ///< No input device
            KEYBOARD, ///< Keyboard input
            MOUSE, ///< Mouse buttons
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

        /// Get how far the cursor has traveled this frame.
        /**
         * @return distance traveled in X by cursor
         */
        double DeltaCursorX() const;

        /// Get how far the cursor has traveled this frame.
        /**
         * @return distance traveled in Y by cursor
         */
        double DeltaCursorY() const;

        /// Assign a button binding.
        /**
         * See <a href="http://www.glfw.org/docs/latest/group__keys.html">GLFW keyboard documentation</a> for indices for keys.
         * For mouse buttons, either GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT or GLFW_MOUSE_BUTTON_MIDDLE.
         * See Input.hpp for XBOX buttons.
         * @param player Player for which the binding is valid.
         * @param button Which button to bind.
         * @param device Which device (KEYBOARD, MOUSE or JOYSTICK).
         * @param index Index of the key.
         * @param axis Whether the button is a joystick axis.
         */
        void AssignButton(Player player, Button button, Device device, int index, bool axis=false);
        
        /// Gets the value of an axis on joystick.
        /**
         * @param player Which player to check.
         * @param button The button to check.
         * @return a float with a value between -1 and 1 - representing axis value.
         */
        double ButtonValue(Player player, Button button) const;
        
        /// Gets whether a button is currently down.
        /**
         * @param player Which player to check.
         * @param button The button to check.
         * @return Whether the button is down
         */
        bool Pressed(Player player, Button button);
        
        /// Gets whether a button was just pressed.
        /**
         * Checks whether a button was pressed between the last two calls to update().
         * @param player Which player to check.
         * @param button The button to check.
         * @return Whether the button was pressed
         */
        bool Triggered(Player player, Button button);
        
        /// Gets whether a button was just released.
        /**
         * Checks whether a button was released between the last two calls to update().
         * @param player Which player to check.
         * @param button The button to check.
         * @return Whether the button was released
         */
        bool Released(Player player, Button button);
        
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
        
        /// Check if the joystick is active/connected.
        /**
         * @param player Player for whom to check joystick.
         * @return Whether joystick is active or not.
         */
        bool JoystickActive(Player player);

        /// Get the last valid aim 
        /**
         * @param player for whom to retrieve last valid direction.
         * @return vector representing direction.
         */
        glm::vec2 LastValidAimDirection(Player player) const;

        /// Get the last valid aim 
        /**
         * @param player for whom to set last valid direction.
         * @param direction to store.
         */
        void SetLastValidAimDirection(Player player, glm::vec2 direction);

        /// Get the deadzone for the aiming axis on the controller
        /**
         * @return The deadzone for aiming.
         */
        double AimDeadzone() const;

        /// Get the deadzone for the moving axis on the controller
        /**
         * @return The deadzone for moving.
         */
        double MoveDeadzone() const;

        /// Set the deadzone for the aiming axis on the controller
        /**
         * @param aimDeadzone The deadzone for aiming.
         */
        void SetAimDeadzone(double aimDeadzone);

        /// Set the deadzone for the moving axis on the controller
        /**
         * @param moveDeadzone The deadzone for moving.
         */
        void SetMoveDeadzone(double moveDeadzone);
        
    private:
        static InputHandler* mActiveInstance;
        
        GLFWwindow* mWindow;
        
        // Bindings
        struct Binding {
            Player player;
            Button button;
            Device device;
            int index;
            bool axis;
        };
        std::vector<Binding> mBindings;

        // Data
        struct ButtonData {
            double value;
            bool released;
            bool triggered;
        };
        
        // Button data.
        ButtonData mButtonData[PLAYERS][BUTTONS];
        
        // Mouse states.
        double mCursorX, mCursorY;
        double mDeltaCursorX, mDeltaCursorY;
        double mLastScroll;
        double mScroll;

        // Stores the last valid direction for player aiming.
        glm::vec2 mLastValidAimDirection[PLAYERS];
        
        // Text input.
        std::string mText, mTempText;
        
        // Whether the joysticks are active.
        bool mJoystickActive[PLAYERS - 1];
        
        // Joystick thresholds
        double mMoveDeadzone;
        double mAimDeadzone;
};

/// Get currently active input handler.
/**
 * @return The currently active input handler or nullptr.
 */
InputHandler* Input();
