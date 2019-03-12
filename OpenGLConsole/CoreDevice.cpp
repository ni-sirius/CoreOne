#include "stdafx.h"
#include "CoreDevice.h"
#include "CoreCommands.h"


void CoreDevice::AddCommand(CoreBaseCommand* command)
{
  std::lock_guard<std::mutex> lock(_stackMut);
  _commandsStack.emplace_back(command);
}

void CoreDevice::ProcessAll()
{
  //_stackMut.lock();
  std::lock_guard<std::mutex> lock(_stackMut);
  while (_commandsStack.size() > 0)
  {
    _commandsStack.front()->Execute();
    _commandsStack.pop_front();
  }
}
