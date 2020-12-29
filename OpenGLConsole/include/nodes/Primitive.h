#pragma once
#include <vector>
#include <glfw3.h>

struct Vertex;

namespace coreone
{

  class Primitive
  {
  public:
    void Set(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
    void Set(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices);
    void Set(const Vertex* verices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices);

    const std::vector<Vertex>& GetVerticesVec() const;
    const std::vector<GLuint>& GetIndicesVec() const;
    Vertex* GetVertices();
    GLuint* GetIndices();
    const unsigned GetNrOfVertices();
    const unsigned GetNrOfIndices();

  private:
    std::vector<Vertex> _vertices;
    std::vector<GLuint> _indices;
  };

  class Quad : public Primitive
  {
  public:
    Quad();
  };

  class Triangle : public Primitive
  {
  public:
    Triangle();
  };

  class Pyramid : public Primitive
  {
  public:
    Pyramid();
  };

  class Cube : public Primitive
  {
  public:
    Cube();
  };

}