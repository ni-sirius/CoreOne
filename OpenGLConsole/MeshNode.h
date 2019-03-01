#pragma once
#include "CoreNode.h"

struct Vertex;
class Shader;
class Primitive;

class MeshNode : public CoreNode
{
public:
  MeshNode(Vertex* vertexArray,
           const unsigned int& numOfVertices,
           GLuint* indexArray,
           const unsigned int& numOfIndices,
           glm::vec3 position,
           glm::vec3 rotation,
           glm::vec3 scale,
           std::shared_ptr<Material> material);

  MeshNode(std::shared_ptr<Primitive> primitive,
           glm::vec3 position,
           glm::vec3 rotation,
           glm::vec3 scale,
           std::shared_ptr<Material> material);
  virtual ~MeshNode();

  virtual void Update(glm::mat4 modelMatrix = glm::mat4(1.f)) override;
  virtual void Render(Shader* shader, ShaderPass passType) override;

  inline void SetPosition(const glm::vec3 position) { _position = position; };
  inline void SetRotation(const glm::vec3 rotation) { _rotation = rotation; };
  inline void SetScale(const glm::vec3 scale) { _scale = scale; };

  inline void Move(const glm::vec3 position) { _position += position; };
  inline void Rotate(const glm::vec3 rotation) { _rotation += rotation; };
  inline void Upscale(const glm::vec3 upscale) { _scale += upscale; };

  inline void SetMaterial(std::shared_ptr<Material> material) { _material = material; }
protected:
  void initVAO(Vertex* vertexArray,
              const unsigned& numOfVertices,
              GLuint* indexArray,
              const unsigned& numOfIndices);
  void initVAO(std::shared_ptr<Primitive> primitive);

  void updateModelMatrix();
  void updateUniforms(Shader* shader);

protected:
  glm::vec3 _position;
  glm::vec3 _rotation;
  glm::vec3 _scale;

  GLuint _VAO;
  GLuint _VBO;
  GLuint _EBO;

  unsigned int _numOfVertices;
  unsigned int _numOfIndices;

  std::shared_ptr<Material> _material;
};

