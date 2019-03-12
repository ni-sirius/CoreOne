#pragma once
#include <mutex>
#include <deque>
#include "CoreCommands.h"


class CoreDevice
{
public:
  CoreDevice() = default;
  ~CoreDevice() = default;

  void AddCommand(CoreBaseCommand* command);

  void ProcessAll();

private:
  std::deque<std::unique_ptr<CoreBaseCommand>> _commandsStack;
  std::mutex _stackMut;
};

