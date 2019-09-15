#include "stdafx.h"
#include "states/CoreState.h"


void CoreState::setState()
{
  _preExecuteCallback();
}

void CoreState::restoreState()
{
  _postExecuteCallback();
}
