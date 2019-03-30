#include "stdafx.h"
#include "Game.h"
#include "GameStates.h"


Game::Game(int GLMajorVer, int GLMinorVer):
  _glMajorVer(GLMajorVer),
  _glMinorVer(GLMinorVer),
  _core(new Core("Game test", 1024, 768, _glMajorVer, _glMinorVer, false)),
  _currentActiveState(MENU)
{
  //Camera
  auto camera = std::make_shared<Camera>(glm::vec3(0.f, 0.f, 3.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), true);
  _core->SetCamera(camera, 90.f, 0.1f, 1000.f);

  initStates();
  SetActiveState(MENU);

  render();//move to another thread
}

Game::~Game()
{
}

void Game::SetActiveState(int activeState)
{
  _states[_currentActiveState]->Disactivate();

  _states[activeState]->Activate();
  _currentActiveState = static_cast<State>(activeState);
}

void Game::render()
{
  //Main loop
  while (!_core->GetWindiwShouldClose())
  {
    _core->Update();
    _core->Render();
  }
}

void Game::initStates()
{
  auto changeStateCallback = std::bind(&Game::SetActiveState, this, std::placeholders::_1);

  _states.push_back(std::make_shared<GameStateMenu>(_core, changeStateCallback));
  _states.push_back(std::make_shared<GameStatePlay>(_core, changeStateCallback));
  _states.push_back(std::make_shared<GameStateScores>(_core, changeStateCallback, &_scoresTable));
  _states.push_back(std::make_shared<GameStateEndgame>(_core, changeStateCallback, &_scoresTable));
}
