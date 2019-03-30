#pragma once
#include <glfw3.h>
#include <functional>
#include <map>

class CoreWindow
{
public:
  CoreWindow(int windowWidth, int windowHeight, int glMajorVer, int glMinorVer);
  virtual ~CoreWindow();

  bool InitWindow(std::string title, bool resizable);

  bool GetWindowShouldClose();
  void SetWindowShouldClose(bool close);

  GLFWwindow* Window();

  void AddKeyCallback(int key, std::function<void()> callback);
  void RemoveKeyCallback(int key);
  void RunKeyboardCallbacks(GLFWwindow* window, int key, int scancode, int action, int mode);

  void HideCoursor(bool hide);

  std::pair<int, int> GetFramebufferSize();

  void SwapBuffers();

private:
  bool initGLFW();
  bool initWindow(std::string title, bool resizable);

private:
  GLFWwindow * _window;
  const int _windowWidth;
  const int _windowHeight;
  int _framebufferWidth;
  int _frameBufferHeight;

  //OpenGL context
  int _glMajorVer;
  int _glMinorVer;

  std::map<int, std::function<void()>> _keyMap;
};

