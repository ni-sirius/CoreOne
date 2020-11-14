#include "stdafx.h"
#include "CoreWindow.h"

using namespace coreone::corewindow;

CoreWindow::CoreWindow()
{
}

CoreWindow::~CoreWindow()
{
  glfwDestroyWindow(_window);
  glfwTerminate();
}

bool CoreWindow::Init(int windowWidth, int windowHeight,
  int glMajorVer, int glMinorVer, std::string title, bool resizable)
{
  if (initGLFW() && initWindow(windowWidth, windowHeight, glMajorVer, glMinorVer, title, resizable))
  {
    glfwSetWindowUserPointer(_window, this);

    glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
      CoreWindow* coreWnd = static_cast<CoreWindow*>(glfwGetWindowUserPointer(window));
      coreWnd->RunKeyboardCallbacks(window, key, scancode, action, mode);
      });

    _initialized = true;
  }

  return _initialized;
}

bool CoreWindow::Initialized() const
{
  return _initialized;
}

std::pair<int, int>CoreWindow::GetGlVersiion() const
{
  int glMajor = glfwGetWindowAttrib(_window, GLFW_CONTEXT_VERSION_MAJOR);
  int glMinor = glfwGetWindowAttrib(_window, GLFW_CONTEXT_VERSION_MINOR);
  return std::make_pair(glMajor, glMinor);
}

bool CoreWindow::GetWindowShouldClose() const
{
  return glfwWindowShouldClose(_window);
}

void CoreWindow::SetWindowShouldClose(bool close/* = true*/) const
{
  glfwSetWindowShouldClose(_window, close);
}

GLFWwindow* CoreWindow::Window() const
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

void CoreWindow::RunKeyboardCallbacks(GLFWwindow* window, int key, int scancode, int action, int mode) const
{
  if (_keyMap.count(key) == 0)
    return;

  if (action == GLFW_PRESS)
  {
    auto it = _keyMap.find(key);
    if (it != _keyMap.end())
    {
      const auto& callback = it->second;
      callback();
    }
  }
}

void CoreWindow::HideCoursor(bool hide) const
{
  auto hideState = hide ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
  glfwSetInputMode(_window, GLFW_CURSOR, hideState);
}

std::pair<int, int> CoreWindow::GetFramebufferSize() const
{
  int framebufferWidth, frameBufferHeight;
  glfwGetFramebufferSize(_window, &framebufferWidth, &frameBufferHeight); //for projection matrix
  return std::make_pair(framebufferWidth, frameBufferHeight);
}

void CoreWindow::SwapBuffers() const
{
  glfwSwapBuffers(_window);
}

bool CoreWindow::initGLFW() const
{
  if (glfwInit() == GLFW_FALSE)
  {
    std::cout << "CORE::GLFW::ERROR Init failed" << std::endl;
    glfwTerminate();

    return false;
  }

  return true;
}

bool CoreWindow::initWindow(int windowWidth, int windowHeight,
  int glMajorVer, int glMinorVer, std::string title, bool resizable)
{
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, glMajorVer);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, glMinorVer);
  glfwWindowHint(GLFW_RESIZABLE, resizable);

  _window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), nullptr, nullptr);
  if (!_window)
  {
    std::cout << "WINDOW::ERROR Init failed" << std::endl;
    glfwTerminate();
    return false;
  }

  if (resizable)
  {
    auto framebufferResizeCallback = [](GLFWwindow* window, int frameBufWidth, int frameBufHeight) {
      glViewport(0, 0, frameBufWidth, frameBufHeight);
    };
    glfwSetFramebufferSizeCallback(_window, framebufferResizeCallback);
  }
  else
  {
    //set up framebuffer size once if static size
    glViewport(0, 0, windowWidth, windowHeight);
  }

  glfwMakeContextCurrent(_window);

  return true;
}
