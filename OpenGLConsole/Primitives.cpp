#include "stdafx.h"
#include "Primitives.h"


Primitive::Primitive()
{

}

Primitive::~Primitive()
{

}

void Primitive::Set(const Vertex* verices, const unsigned nrOfVertices, const GLuint* indices, const unsigned nrOfIndices)
{
  for (size_t i = 0; i < nrOfVertices; i++)
  {
    _vertices.push_back(verices[i]);
  }
  for (size_t i = 0; i < nrOfIndices; i++)
  {
    _indices.push_back(indices[i]);
  }
}

Vertex* Primitive::GetVertices()
{
  return _vertices.data();
}

GLuint* Primitive::GetIndices()
{
  return _indices.data();
}

const unsigned Primitive::GetNrOfVertices()
{
  return _vertices.size();
}

const unsigned Primitive::GetNrOfIndices()
{
  return _indices.size();
}


//QUAD
Quad::Quad():
  Primitive()
{
  Vertex vertices[] = {
    //position                     //color                    //uvcoords               //normals                 
    glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f),      glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f),      glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),      glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(0.5f, 0.5f, 0.0f),   glm::vec3(1.f, 1.f, 0.f), glm::vec2(1.f, 1.f),      glm::vec3(0.f, 0.f, 1.f)
  };
  unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

  GLuint indices[] = {
    0, 1, 2,
    0, 2, 3
  };
  unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

  Set(vertices, nrOfVertices, indices, nrOfIndices);
}

//Triangles
Triangle::Triangle()
{
  Vertex vertices[] = {
    //position                     //color                    //uvcoords               //normals                 
    glm::vec3(-0.5f, 0.5f, 0.0f),  glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 1.f),      glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f),      glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(0.5f, -0.5f, 0.0f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),      glm::vec3(0.f, 0.f, 1.f)
  };
  unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

  GLuint indices[] = {
    0, 1, 2
  };
  unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

  Set(vertices, nrOfVertices, indices, nrOfIndices);
}