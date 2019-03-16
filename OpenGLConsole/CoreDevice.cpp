#include "stdafx.h"
#include "CoreDevice.h"
#include "CoreCommands.h"


CoreDevice::CoreDevice() :
  _isRunning(true)
{
}

CoreDevice::~CoreDevice()
{
  _isRunning = false;
}

void CoreDevice::AddCommand(CoreBaseCommand* command)
{
  if (!_isRunning)
    return;

  std::lock_guard<std::mutex> lock(_stackMut);
  _commandsStack.emplace_back(command);
}

void CoreDevice::ProcessAll()
{
  if (!_isRunning)
    return;

  std::lock_guard<std::mutex> lock(_stackMut);
  while (_commandsStack.size() > 0)
  {
    _commandsStack.front()->Execute();
    _commandsStack.pop_front();
  }
}
