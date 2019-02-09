#pragma once
#include <vector>
#include <glfw3.h>

struct Vertex;

class Primitive
{
public:
  Primitive();
  ~Primitive();

  void Set(const Vertex* verices,
           const unsigned nrOfVertices,
           const GLuint* indices,
           const unsigned nrOfIndices);

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