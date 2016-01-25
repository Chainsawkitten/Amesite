#pragma once

struct GLFWwindow;

/// The main game window in which the game is contained.
class MainWindow {
public:
    /// Create new main window.
    /**
     * @param width Width of the window in pixels.
     * @param height Height of the window in pixels.
     * @param title Window title.
     */
    MainWindow(int width, int height, const char* title = "");
    
    /// Destructor.
    ~MainWindow();
    
    /// Initialize components.
    void Init();
    
    /// Get whether the window should close.
    /**
     * @return Whether the window should close
     */
    bool ShouldClose() const;
    
    /// Swap front- and backbuffers.
    void SwapBuffers();
    
private:
    GLFWwindow* mWindow;
};
