#include "stdafx.h"
#include "CoreWindow.h"


CoreWindow::CoreWindow(int windowWidth, int windowHeight, int glMajorVer, int glMinorVer):
  _windowWidth(windowWidth),
  _windowHeight(windowHeight),
  _framebufferWidth(0),
  _frameBufferHeight(0),
  _glMajorVer(glMajorVer),
  _glMinorVer(glMinorVer)
{
}

CoreWindow::~CoreWindow()
{
  glfwDestroyWindow(_window);
  glfwTerminate();
}

bool CoreWindow::InitWindow(std::string title, bool resizable)
{
  bool result(false);

  if (initGLFW())
    if (initWindow(title, resizable))
    {
      glfwSetWindowUserPointer(_window, this);

      glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
        CoreWindow* coreWnd = static_cast<CoreWindow*>(glfwGetWindowUserPointer(window));
        coreWnd->RunKeyboardCallbacks(window, key, scancode, action, mode);
      });

      result = true;
    }

  return result;
}

GLFWwindow* CoreWindow::Window()
{
  return _window;
}

void CoreWindow::AddKeyCallback(int key, std::function<void()> callback)
{
  _keyMap[key] = callback;
}

void CoreWindow::RemoveKeyCallback(int key)
{
  _keyMap.erase(key);
}

void CoreWindow::RunKeyboardCallbacks(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if (_keyMap.count(key) == 0)
    return;

  if (action == GLFW_PRESS)
    _keyMap[key]();

  //test
  //static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  //  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  //    glfwSetWindowShouldClose(window, GL_TRUE);
  //}
}

void CoreWindow::HideCoursor(bool hide)
{
  auto hideState = hide ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
  glfwSetInputMode(_window, GLFW_CURSOR, hideState);
}

std::pair<int, int> CoreWindow::GetFramebufferSize()
{
  return std::make_pair(_framebufferWidth, _frameBufferHeight);
}

bool CoreWindow::initGLFW()
{
  if (glfwInit() == GLFW_FALSE)
  {
    std::cout << "CORE::GLFW::ERROR Init failed" << std::endl;
    glfwTerminate();

    return false;
  }

  return true;
}

bool CoreWindow::initWindow(std::string title, bool resizable)
{
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _glMajorVer);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _glMinorVer);
  glfwWindowHint(GLFW_RESIZABLE, resizable);

  _window = glfwCreateWindow(_windowWidth, _windowHeight, title.c_str(), nullptr, nullptr);
  if (!_window)
  {
    std::cout << "WINDOW::ERROR Init failed" << std::endl;
    glfwTerminate();
    return false;
  }

  glfwGetFramebufferSize(_window, &_framebufferWidth, &_frameBufferHeight); //for projection matrix

  auto framebufferResizeCallback = [](GLFWwindow* window, int frameBufWidth, int frameBufHeight) {
    glViewport(0, 0, frameBufWidth, frameBufHeight);
  };
  glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
  //glViewport(0, 0, framebufferWidth, frameBufferHeight); if static size

  glfwMakeContextCurrent(_window);

  return true;
}
