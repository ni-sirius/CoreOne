#pragma once
#include "Core.h"
#include <thread>

class GameStatesBase;

class Game final
{
  enum State {MENU, PLAY, SCORES, ENDGAME};

public:
  Game(int GLMajorVer, int GLMinorVer);
  ~Game();

  void SetActiveState(int activeState);

private:
  void render();

  void initStates();

private:
  int _glMajorVer,
      _glMinorVer;

  std::vector<std::shared_ptr<GameStatesBase>> _states;
  State _currentActiveState;

  std::shared_ptr<Core> _core;

  std::multimap<std::string, int> _scoresTable;
};
