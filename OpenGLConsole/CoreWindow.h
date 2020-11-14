#pragma once
#include <glfw3.h>
#include <functional>
#include <map>

namespace coreone::corewindow
{
  class CoreWindow final
  {
  public:
    CoreWindow();
    virtual ~CoreWindow();

    bool Init(int windowWidth, int windowHeight,
      int glMajorVer, int glMinorVer,
      std::string title, bool resizable);

    bool Initialized() const;

    std::pair<int, int> GetGlVersiion() const;

    bool GetWindowShouldClose() const;
    void SetWindowShouldClose(bool close = true) const;

    GLFWwindow* Window() const;

    void AddKeyCallback(int key, std::function<void()> callback);
    void RemoveKeyCallback(int key);
    void RunKeyboardCallbacks(GLFWwindow* window, int key, int scancode, int action, int mode) const;

    void HideCoursor(bool hide) const;

    std::pair<int, int> GetFramebufferSize() const;

    void SwapBuffers() const;

  private:
    bool initGLFW() const;
    bool initWindow(int windowWidth, int windowHeight,
      int glMajorVer, int glMinorVer, 
      std::string title, bool resizable);

  private:
    GLFWwindow * _window;
    bool _initialized = false;

    std::map<int, std::function<void()>> _keyMap;
  };

} //namespace coreone::corewindow
