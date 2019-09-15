#pragma once
//#include <functional>

class MeshNode;

class CoreAttribute
{
public:
  enum class Type { PRE_CALL = 0, POST_CALL };

  CoreAttribute() = default;
  virtual ~CoreAttribute() = default;

  virtual Type GetType() const = 0;
};

class CoreState final
{
  using AttribPtr = std::shared_ptr<CoreAttribute>;

public:
  CoreState() = default;
  ~CoreState() = default;

  //TODO Rewrite attribute systems
  //void SetAttribute(AttribPtr attr);
  inline void SetPreExecCallback(std::function<void(void)> callback) { _preExecuteCallback = callback; }
  inline void SetPostExecCallback(std::function<void(void)> callback) {_postExecuteCallback = callback;}

private:
  void setState();
  void restoreState();

private:
  friend MeshNode;

  std::weak_ptr<MeshNode> _parent;

  std::function<void(void)> _preExecuteCallback;
  std::function<void(void)> _postExecuteCallback;
};