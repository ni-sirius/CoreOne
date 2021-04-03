#pragma once
#include <vector>
#include <glfw3.h>

struct Vertex;

namespace coreone
{

  class Primitive
  {
  public:
    ~Primitive();

    void Set(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
    void Set(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices);
    void Set(const Vertex* verices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices);

    const std::vector<Vertex>& GetVerticesVec() const;
    const std::vector<GLuint>& GetIndicesVec() const;
    const Vertex* GetVertices() const;
    const GLuint* GetIndices() const;
    const unsigned GetNrOfVertices() const;
    const unsigned GetNrOfIndices() const;

    const bool Empty() const;

    void DrawPrimitive() const;

  private:
    void initGlData();
    void clearGlData();

  private:
    std::vector<Vertex> _vertices;
    std::vector<GLuint> _indices;

    GLuint _VAO;
    GLuint _VBO;
    GLuint _EBO;
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