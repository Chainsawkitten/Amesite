#pragma once

struct GLFWwindow;

/// The main game window in which the game is contained.
class MainWindow {
    public:
        /// Create new main window.
        /**
         * @param width Width of the window in pixels.
         * @param height Height of the window in pixels.
         * @param fullscreen Whether to start in fullscreen mode.
         * @param borderless Whether to use a borderless window.
         * @param title Window title.
         * @param debugContext Whether to activate OpenGL debug context.
         */
        MainWindow(int width, int height, bool fullscreen = false, bool borderless = false, const char* title = "", bool debugContext = false);
        
        /// Destructor.
        ~MainWindow();
        
        /// Initialize components.
        void Init();
        
        /// Set window title.
        /**
         * @param title New window title.
         */
        void SetTitle(const char* title);
        
        /// Get whether the window should close.
        /**
         * @return Whether the window should close
         */
        bool ShouldClose() const;
        
        /// Swap front- and backbuffers.
        void SwapBuffers();
        
    private:
        GLFWwindow* mWindow;
        bool mDebugContext;
};
