#include "stdafx.h"
#include "states/CoreState.h"


void CoreState::setState()
{
  if (_preExecuteCallback)
    _preExecuteCallback();
}

void CoreState::restoreState()
{
  if (_postExecuteCallback)
    _postExecuteCallback();
}
