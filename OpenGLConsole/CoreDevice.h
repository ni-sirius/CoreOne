#pragma once
#include <mutex>
#include <deque>
#include "CoreCommands.h"


class CoreDevice
{
public:
  CoreDevice();
  ~CoreDevice();

  void AddCommand(CoreBaseCommand* command);

  void ProcessAll();

private:
  bool _isRunning;

  std::deque<std::unique_ptr<CoreBaseCommand>> _commandsStack;
  std::mutex _stackMut;
};

