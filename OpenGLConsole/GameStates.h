#pragma once
#include "Core.h"
#include "TextNode.h"
#include "MeshNode.h"

class GameStatesBase
{
public:
  GameStatesBase(std::shared_ptr<Core> core, std::function<void(int)> changeStateCallback);
  virtual ~GameStatesBase() = default;

  virtual void Activate() = 0;
  virtual void Disactivate() = 0;

  inline std::function<void(int)> GetChangeStateCallback() { return _changeStateCallback; }

protected:
  void loadResources();

  std::shared_ptr<Core> _core;

  std::function<void(int)> _changeStateCallback;

  std::vector< std::shared_ptr<Material> > _materials;
  std::vector< std::shared_ptr<Texture> > _textures;
};

class GameStateMenu: public GameStatesBase
{
  enum TextNodes { MENU, PLAY, SCORES, EXIT };

public:
  GameStateMenu(std::shared_ptr<Core> core, std::function<void(int)> changeStateCallback);
  ~GameStateMenu();

  virtual void Activate();
  virtual void Disactivate();

  size_t GetCurrentSelectedText();
  void SelectText(TextNodes textNode);

private:
  bool _activated;

  const float _normalSize = 1.f;
  const float _selectedSize = 1.5f;

  std::vector<std::shared_ptr<TextNode>> _texts;
  size_t _activeText;
};

class GameStatePlay : public GameStatesBase
{
public:
  GameStatePlay(std::shared_ptr<Core> core, std::function<void(int)> changeStateCallback);
  ~GameStatePlay();

  virtual void Activate();
  virtual void Disactivate();

private:
  bool _activated;

  std::shared_ptr<MeshNode> _flower;
  std::vector<std::shared_ptr<MeshNode>> _boxes;
};

class GameStateScores : public GameStatesBase
{
public:
  GameStateScores(std::shared_ptr<Core> core, std::function<void(int)> changeStateCallback, std::multimap<std::string, int>* scoresTable);
  ~GameStateScores();

  virtual void Activate();
  virtual void Disactivate();

private:
  bool _activated;

  std::shared_ptr<TextNode> _scoresTitleText;
  std::vector<std::shared_ptr<TextNode>> _scores;

  std::multimap<std::string, int>* _scoresTable;
};

class GameStateEndgame : public GameStatesBase
{
public:
  GameStateEndgame(std::shared_ptr<Core> core, std::function<void(int)> changeStateCallback, std::multimap<std::string, int>* scoresTable);
  ~GameStateEndgame();

  virtual void Activate();
  virtual void Disactivate();

private:
  bool _activated;

  std::shared_ptr<TextNode> _endgameTitleText;

  std::multimap<std::string, int>* _scoresTable;
};

