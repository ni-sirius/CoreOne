#include "stdafx.h"
#include "MeshNode.h"


MeshNode::MeshNode(Vertex* vertexArray,
                   const unsigned int& numOfVertices,
                   GLuint* indexArray,
                   const unsigned int& numOfIndices,
                   glm::vec3 position,
                   glm::vec3 rotation,
                   glm::vec3 scale,
                   std::shared_ptr<Material> material /*= nullptr*/,
                   std::shared_ptr<Texture> diffuseTexture /*= nullptr*/,
                   std::shared_ptr<Texture> specularTexture /*= nullptr*/):
  _position(position),
  _rotation(rotation),
  _scale(scale),
  _numOfVertices(numOfVertices),
  _numOfIndices(numOfIndices),
  _material(material),
  _diffuseTexture(diffuseTexture),
  _specularTexture(specularTexture)
{
  initVAO(vertexArray, numOfVertices, indexArray, numOfIndices);
  updateModelMatrix();
}

MeshNode::MeshNode(std::shared_ptr<Primitive> primitive,
                   glm::vec3 position,
                   glm::vec3 rotation,
                   glm::vec3 scale,
                   std::shared_ptr<Material> material /*= nullptr*/,
                   std::shared_ptr<Texture> diffuseTexture /*= nullptr*/,
                   std::shared_ptr<Texture> specularTexture /*= nullptr*/):
  _position(position),
  _rotation(rotation),
  _scale(scale),
  _numOfVertices(primitive->GetNrOfVertices()),
  _numOfIndices(primitive->GetNrOfIndices()),
  _material(material),
  _diffuseTexture(diffuseTexture),
  _specularTexture(specularTexture)
{
  initVAO(primitive);
  updateModelMatrix();
}

MeshNode::~MeshNode()
{
}

void MeshNode::Update(glm::mat4 modelMatrix /*= glm::mat4(1.f)*/)
{
  updateModelMatrix();

  CoreNode::Update(modelMatrix);
}

void MeshNode::Render(Shader* shader, ShaderPass passType)
{
  if (passType == ShaderPass::MESH_PASS)
  {
    if (_material)
    {
      _material->SendToShader(*shader);

      if (_diffuseTexture)
      {
        _material->UseColors(false);
        _diffuseTexture->Bind(_material->GetDiffuseTexUnit());
      }
      else
        _material->UseColors(true);

      if (_specularTexture)
        _specularTexture->Bind(_material->GetSpecularTexUnit());
    }

    updateUniforms(shader);

    shader->Use();

    //bind VAO
    glBindVertexArray(_VAO);

    //draw
#pragma warning( push )
#pragma warning( disable : 4267)
    if (_numOfIndices)
      glDrawElements(GL_TRIANGLES, _numOfIndices, GL_UNSIGNED_INT, 0);
    else
      glDrawArrays(GL_TRIANGLES, 0, _numOfVertices);
#pragma warning( pop )
  }

  if (_diffuseTexture)
    _diffuseTexture->Unbind();

  if (_specularTexture)
    _specularTexture->Unbind();

  CoreNode::Render(shader, passType);
}

void MeshNode::initVAO(Vertex* vertexArray,
                       const unsigned& numOfVertices,
                       GLuint* indexArray,
                       const unsigned& numOfIndices)
{
  //VAO VBO EBO, Generation of
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, numOfVertices * sizeof(Vertex), vertexArray, GL_STATIC_DRAW);

  if (numOfIndices > 0)
  {
    glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfIndices * sizeof(GLuint), indexArray, GL_STATIC_DRAW);
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

void MeshNode::initVAO(std::shared_ptr<Primitive> primitive)
{
  initVAO(primitive->GetVertices(), primitive->GetNrOfVertices(), primitive->GetIndices(), primitive->GetNrOfIndices());
}

void MeshNode::updateModelMatrix()
{
  _modelMatrix = glm::mat4(1.f);
  _modelMatrix = glm::translate(_modelMatrix, _position);
  _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.x), glm::vec3(1.f, 0.f, 0.f));
  _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.y), glm::vec3(0.f, 1.f, 0.f));
  _modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.z), glm::vec3(0.f, 0.f, 1.f));
  _modelMatrix = glm::scale(_modelMatrix, _scale);
}

void MeshNode::updateUniforms(Shader* shader)
{
  shader->SetMat4fv(_modelMatrix, "ModelMat");
}
