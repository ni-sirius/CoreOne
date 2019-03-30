#include "stdafx.h"
#include "GameStates.h"
#include "CoreCommands.h"

GameStatesBase::GameStatesBase(std::shared_ptr<Core> core, std::function<void(int)> changeStateCallback):
  _core(core),
  _changeStateCallback(changeStateCallback)
{
  loadResources();
}

void GameStatesBase::loadResources()
{
  auto windshieldShader = std::make_shared<Shader>(4, 5, "windshield.vert", "windshield.frag");
  _materials.push_back(std::make_shared<Material>(glm::vec3(1.0f), 32, 0, 1, windshieldShader));

  auto textShader = std::make_shared<Shader>(4, 5, "text.vert", "text.frag");
  _materials.push_back(std::make_shared<Material>(glm::vec3(1.0f), 32, 0, 1, textShader));

  _textures.push_back(std::make_shared<Texture>("Images/container.png", GL_TEXTURE_2D));
  _textures.push_back(std::make_shared<Texture>("Images/flower.png", GL_TEXTURE_2D));
}

GameStateMenu::GameStateMenu(std::shared_ptr<Core> core, std::function<void(int)> changeStateCallback):
  GameStatesBase(core, changeStateCallback),
  _activated(false),
  _activeText(PLAY)
{
  auto menuText = std::make_shared<TextNode>("Main Menu", glm::vec3(600.f, 600.f, 0.f), _normalSize, _materials[1], false);
  _texts.push_back(menuText);

  auto startText = std::make_shared<TextNode>("Play", glm::vec3(25.f, 225.f, 0.f), _normalSize, _materials[1], false);
  _texts.push_back(startText);

  auto scoreText = std::make_shared<TextNode>("Scores", glm::vec3(25.f, 125.f, 0.f), _normalSize, _materials[1], false);
  _texts.push_back(scoreText);

  auto exitText = std::make_shared<TextNode>("Exit", glm::vec3(25.f, 25.f, 0.f), _normalSize, _materials[1], false);
  _texts.push_back(exitText);

  for (auto text : _texts)
  {
    _core->AddTextSceneNode(text);
  }
}

GameStateMenu::~GameStateMenu()
{
}

void GameStateMenu::Activate()
{
  for (size_t i = 0; i < _texts.size(); i++)
  {
    auto size = _activeText == i ? _selectedSize : _normalSize;

    auto scaleCommand = new SetTextScaleCommand(size, _texts[i]);
    _core->AddCommand(scaleCommand);

    auto showCommand = new SetTextNodeVisibleCommand(true, _texts[i]);
    _core->AddCommand(showCommand);
  }

  auto selectDownCallback = [this]() {
    int selected = this->GetCurrentSelectedText();
    selected++;
    selected = std::max(1, std::min(selected, 3));

    this->SelectText(static_cast<TextNodes>(selected));
  };
  _core->RegisterKeyCallback(GLFW_KEY_DOWN, selectDownCallback);

  auto selectUpCallback = [this]() {
    int selected = this->GetCurrentSelectedText();
    selected--;
    selected = std::max(1, std::min(selected, 3));

    this->SelectText(static_cast<TextNodes>(selected));
  };
  _core->RegisterKeyCallback(GLFW_KEY_UP, selectUpCallback);

  auto enterCallback = [this]() {
    int selected = this->GetCurrentSelectedText();

    switch (selected)
    {
    case PLAY:
      this->GetChangeStateCallback()(1);
      break;
    case SCORES:
      this->GetChangeStateCallback()(2);
      break;
    case EXIT:
      _core->SetWindowShouldClose();
      break;
    default:
      break;
    }
  };
  _core->RegisterKeyCallback(GLFW_KEY_ENTER, enterCallback);

  auto escCallback = [this]() {
    _core->SetWindowShouldClose();
  };
  _core->RegisterKeyCallback(GLFW_KEY_ESCAPE, escCallback);

  _activated = true;
}

void GameStateMenu::Disactivate()
{
  if (!_activated)
    return;

  for (auto text : _texts)
  {
    auto command = new SetTextNodeVisibleCommand(false, text);
    _core->AddCommand(command);
  }

  _core->UnregisterKeyCallback(GLFW_KEY_DOWN);
  _core->UnregisterKeyCallback(GLFW_KEY_UP);
  _core->UnregisterKeyCallback(GLFW_KEY_ENTER);
  _core->UnregisterKeyCallback(GLFW_KEY_ESCAPE);

  _activated = false;
}

size_t GameStateMenu::GetCurrentSelectedText()
{
  return _activeText;
}

void GameStateMenu::SelectText(TextNodes textNode)
{
  auto commandDesel = new SetTextScaleCommand(_normalSize, _texts[_activeText]);
  _core->AddCommand(commandDesel);

  auto commandSel = new SetTextScaleCommand(_selectedSize, _texts[textNode]);
  _core->AddCommand(commandSel);

  _activeText = textNode;
}




GameStatePlay::GameStatePlay(std::shared_ptr<Core> core, std::function<void(int)> changeStateCallback):
  GameStatesBase(core, changeStateCallback),
  _activated(false)
{
  _flower = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(100.f, 306.f, 0.f), glm::vec3(0.f), glm::vec3(100.f, 100.f, 0.f), _materials[0], _textures[1], nullptr, false);
  _core->AddWindshieldSceneNode(_flower);

  auto box1_1 = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(512.f, 200.f, 0.f), glm::vec3(0.f), glm::vec3(100.f, 400.f, 0.f), _materials[0], _textures[0], nullptr, false);
  _boxes.push_back(box1_1);
  auto box1_2 = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(512.f, 716.f, 0.f), glm::vec3(0.f), glm::vec3(100.f, 100.f, 0.f), _materials[0], _textures[0], nullptr, false);
  _boxes.push_back(box1_2);

  auto box2_1 = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(812.f, 150.f, 0.f), glm::vec3(0.f), glm::vec3(100.f, 300.f, 0.f), _materials[0], _textures[0], nullptr, false);
  _boxes.push_back(box2_1);
  auto box2_2 = std::make_shared<MeshNode>(std::make_shared<Cube>(), glm::vec3(812.f, 616, 0.f), glm::vec3(0.f), glm::vec3(100.f, 300.f, 0.f), _materials[0], _textures[0], nullptr, false);
  _boxes.push_back(box2_2);

  for (auto mesh : _boxes)
    _core->AddWindshieldSceneNode(mesh);
}

GameStatePlay::~GameStatePlay()
{
}

void GameStatePlay::Activate()
{
  auto commandPlayerVisible = new SetMeshNodeVisibleCommand(true, _flower);
  _core->AddCommand(commandPlayerVisible);

  for (auto mesh : _boxes)
  {
    auto command = new SetMeshNodeVisibleCommand(true, mesh);
    _core->AddCommand(command);
  }

  auto escCallback = [this]() {
      this->GetChangeStateCallback()(0);
  };
  _core->RegisterKeyCallback(GLFW_KEY_ESCAPE, escCallback);

  auto enterCallback = [this]() {
    this->GetChangeStateCallback()(3);
  };
  _core->RegisterKeyCallback(GLFW_KEY_ENTER, enterCallback);

  _activated = true;
}

void GameStatePlay::Disactivate()
{
  if (!_activated)
    return;

  auto commandPlayerVisible = new SetMeshNodeVisibleCommand(false, _flower);
  _core->AddCommand(commandPlayerVisible);

  for (auto mesh : _boxes)
  {
    auto command = new SetMeshNodeVisibleCommand(false, mesh);
    _core->AddCommand(command);
  }

  _core->UnregisterKeyCallback(GLFW_KEY_ESCAPE);

  _activated = false;
}

GameStateScores::GameStateScores(
    std::shared_ptr<Core> core,
    std::function<void(int)> changeStateCallback,
    std::multimap<std::string, int>* scoresTable) :
  GameStatesBase(core, changeStateCallback),
  _scoresTable(scoresTable)
{
  _scoresTitleText = std::make_shared<TextNode>("Best scores", glm::vec3(600.f, 600.f, 0.f), 1.f, _materials[1], false);
  _core->AddTextSceneNode(_scoresTitleText);

  //TODO add scores from a file
  for (auto text : _scores)
  {
    _core->AddTextSceneNode(text);
  }
}

GameStateScores::~GameStateScores()
{
}

void GameStateScores::Activate()
{
  auto autoTitleShowCommand = new SetTextNodeVisibleCommand(true, _scoresTitleText);
  _core->AddCommand(autoTitleShowCommand);

  for (auto text : _scores)
  {
    auto command = new SetTextNodeVisibleCommand(true, text);
    _core->AddCommand(command);
  }

  auto enterEscCallback = [this]() {
      this->GetChangeStateCallback()(0);
  };
  _core->RegisterKeyCallback(GLFW_KEY_ENTER, enterEscCallback);
  _core->RegisterKeyCallback(GLFW_KEY_ESCAPE, enterEscCallback);

  _activated = true;
}

void GameStateScores::Disactivate()
{
  if (!_activated)
    return;

  auto autoTitleShowCommand = new SetTextNodeVisibleCommand(false, _scoresTitleText);
  _core->AddCommand(autoTitleShowCommand);

  for (auto text : _scores)
  {
    auto command = new SetTextNodeVisibleCommand(false, text);
    _core->AddCommand(command);
  }

  _core->UnregisterKeyCallback(GLFW_KEY_ENTER);
  _core->UnregisterKeyCallback(GLFW_KEY_ESCAPE);

  _activated = false;
}

GameStateEndgame::GameStateEndgame(
    std::shared_ptr<Core> core,
    std::function<void(int)> changeStateCallback,
    std::multimap<std::string, int>* scoresTable) :
  GameStatesBase(core, changeStateCallback),
  _scoresTable(scoresTable)
{
  _endgameTitleText = std::make_shared<TextNode>("Game Over", glm::vec3(600.f, 600.f, 0.f), 1.f, _materials[1], false);
  _core->AddTextSceneNode(_endgameTitleText);
}

GameStateEndgame::~GameStateEndgame()
{
}

void GameStateEndgame::Activate()
{
  auto autoTitleShowCommand = new SetTextNodeVisibleCommand(true, _endgameTitleText);
  _core->AddCommand(autoTitleShowCommand);

  //TODO Add score text

  auto enterEscCallback = [this]() {
    this->GetChangeStateCallback()(0);
  };
  _core->RegisterKeyCallback(GLFW_KEY_ENTER, enterEscCallback);
  _core->RegisterKeyCallback(GLFW_KEY_ESCAPE, enterEscCallback);

  _activated = true;
}

void GameStateEndgame::Disactivate()
{
  if (!_activated)
    return;

  auto autoTitleShowCommand = new SetTextNodeVisibleCommand(false, _endgameTitleText);
  _core->AddCommand(autoTitleShowCommand);

  _core->UnregisterKeyCallback(GLFW_KEY_ENTER);
  _core->UnregisterKeyCallback(GLFW_KEY_ESCAPE);

  _activated = false;
}
