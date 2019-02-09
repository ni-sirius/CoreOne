#pragma once
#include <vector>

struct Vertex;
class Shader;
class Primitive;

class Mesh
{
public:
  Mesh(Vertex* vertexArray,
       const unsigned& nrOfVertices,
       GLuint* indexArray,
       const unsigned& nrOfIndices,
       glm::vec3 position = glm::vec3(0.f),
       glm::vec3 rotation = glm::vec3(0.f),
       glm::vec3 scale = glm::vec3(1.f));
  Mesh(Primitive* primitive,
       glm::vec3 position = glm::vec3(0.f),
       glm::vec3 rotation = glm::vec3(0.f),
       glm::vec3 scale = glm::vec3(1.f));
  ~Mesh();

  void SetPosition(const glm::vec3 position);
  void SetRotation(const glm::vec3 rotation);
  void SetScale(const glm::vec3 scale);

  void Move(const glm::vec3 position);
  void Rotate(const glm::vec3 rotation);
  void Upscale(const glm::vec3 upscale);

  void Update();
  void Render(Shader* shader);

private:
  unsigned _nrOfVertices;
  unsigned _nrOfIndices;

  GLuint _VAO;
  GLuint _VBO;
  GLuint _EBO;

  glm::vec3 _position;
  glm::vec3 _rotation;
  glm::vec3 _scale;
  glm::mat4 _modelMatrix;

private:
  void initVAO(Vertex* vertexArray,
               const unsigned& nrOfVertices,
               GLuint* indexArray,
               const unsigned& nrOfIndices);
  void initVAO(Primitive* primitive);

  void updateUniforms(Shader* shader);

  void updateModelMatrix();
};