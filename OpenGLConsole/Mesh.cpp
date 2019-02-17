#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh(Vertex* vertexArray,
           const unsigned& nrOfVertices,
           GLuint* indexArray,
           const unsigned& nrOfIndices,
           glm::vec3 position /*= glm::vec3(0.f)*/,
           glm::vec3 rotation /*= glm::vec3(0.f)*/,
           glm::vec3 scale /*= glm::vec3(1.f)*/):
  _position(position),
  _rotation(rotation),
  _scale(scale)
{
  initVAO(vertexArray, nrOfVertices, indexArray, nrOfIndices);
  updateModelMatrix();
}

Mesh::Mesh(Primitive* primitive,
           glm::vec3 position /*= glm::vec3(0.f)*/,
           glm::vec3 rotation /*= glm::vec3(0.f)*/,
           glm::vec3 scale /*= glm::vec3(1.f)*/):
  _position(position),
  _rotation(rotation),
  _scale(scale)
{
  initVAO(primitive);
  updateModelMatrix();
}

Mesh::~Mesh()
{
  glDeleteVertexArrays(1, &_VAO);
  glDeleteBuffers(1, &_VBO);

  if (_nrOfIndices > 0)
    glDeleteBuffers(1, &_EBO);
}

void Mesh::SetPosition(const glm::vec3 position)
{
  _position = position;
}

void Mesh::SetRotation(const glm::vec3 rotation)
{
  _rotation = rotation;
}

void Mesh::SetScale(const glm::vec3 scale)
{
  _scale = scale;
}

void Mesh::Move(const glm::vec3 position)
{
  _position += position;
}

void Mesh::Rotate(const glm::vec3 rotation)
{
  _rotation += rotation;
}

void Mesh::Upscale(const glm::vec3 upscale)
{
  _scale += upscale;
}

void Mesh::Update()
{

}

void Mesh::Render(Shader* shader)
{
  updateModelMatrix();

  updateUniforms(shader);

  shader->Use();

  //bind VAO
  glBindVertexArray(_VAO);

  //draw
#pragma warning( push )
#pragma warning( disable : 4267)
  if (_nrOfIndices)
    glDrawElements(GL_TRIANGLES, _nrOfIndices, GL_UNSIGNED_INT, 0);
  else
    glDrawArrays(GL_TRIANGLES, 0, _nrOfVertices);
#pragma warning( pop )
}

void Mesh::initVAO(Vertex* vertexArray,
                   const unsigned& nrOfVertices,
                   GLuint* indexArray,
                   const unsigned& nrOfIndices)
{
  _nrOfIndices = nrOfIndices;
  _nrOfVertices = nrOfVertices;

  //VAO VBO EBO, Generation of
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, _nrOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

  if (nrOfIndices > 0)
  {
    glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nrOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);
  }

  //SET vertex attribute pointers and enable(input assembly)
  //Example if there is no known descriptor
  //GLuint attribLoc = glGetAttribLocation(coreProgram, "vertex_position");
  //glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
  //glEnableVertexAttribArray(attribLoc);

  //Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);
  //Color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);
  //Texture
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture));
  glEnableVertexAttribArray(2);
  //Normal
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(3);

  //bind vao 0(unbind enything)
  glBindVertexArray(0);
}

void Mesh::initVAO(Primitive* primitive)
{
  _nrOfIndices = primitive->GetNrOfIndices();
  _nrOfVertices = primitive->GetNrOfVertices();

  //VAO VBO EBO, Generation of
  glCreateVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, _nrOfVertices * sizeof(Vertex), primitive->GetVertices(), GL_STATIC_DRAW);

  if (_nrOfIndices > 0)
  {
    glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nrOfIndices * sizeof(GLuint), primitive->GetIndices(), GL_STATIC_DRAW);
  }

  //SET vertex attribute pointers and enable(input assembly)
  //Example if there is no known descriptor
  //GLuint attribLoc = glGetAttribLocation(coreProgram, "vertex_position");
  //glVertexAttribPointer(attribLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
  //glEnableVertexAttribArray(attribLoc);

  //Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);
  //Color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);
  //Texture
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture));
  glEnableVertexAttribArray(2);
  //Normal
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(3);

  //bind vao 0(unbind enything)
  glBindVertexArray(0);
}

void Mesh::updateUniforms(Shader* shader)
{
  shader->SetMat4fv(_modelMatrix, "ModelMat");
}

void Mesh::updateModelMatrix()
{
  _modelMatrix = glm::mat4(1.f);
  _modelMatrix = glm::translate(_modelMatrix, _position);
  _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.x), glm::vec3(1.f, 0.f, 0.f));
  _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.y), glm::vec3(0.f, 1.f, 0.f));
  _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.z), glm::vec3(0.f, 0.f, 1.f));
  _modelMatrix = glm::scale(_modelMatrix, _scale);
}
