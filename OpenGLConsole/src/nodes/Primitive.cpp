#include <stdafx.h>
#include <nodes/Primitive.h>


using namespace coreone;

void Primitive::Set(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices)
{
  _vertices = vertices;
  _indices = indices;

  if (!Empty())
  {
    clearGlData();
  }
  initGlData();
}

void Primitive::Set(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices)
{
  _vertices = std::move(vertices);
  _indices = std::move(indices);

  if (!Empty())
  {
    clearGlData();
  }
  initGlData();
}

const std::vector<Vertex>& Primitive::GetVerticesVec() const
{
  return _vertices;
}

const std::vector<GLuint>& Primitive::GetIndicesVec() const
{
  return _indices;
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

  if (!Empty())
  {
    clearGlData();
  }
  initGlData();
}

const Vertex* Primitive::GetVertices() const
{
  return _vertices.data();
}

const GLuint* Primitive::GetIndices() const
{
  return _indices.data();
}

const unsigned Primitive::GetNrOfVertices() const
{
  return static_cast<unsigned>(_vertices.size());
}

const unsigned Primitive::GetNrOfIndices() const
{
  return static_cast<unsigned>(_indices.size());
}

void Primitive::DrawPrimitive() const
{
  glBindVertexArray(_VAO);

#pragma warning( push )
#pragma warning( disable : 4267)
  if (GetNrOfIndices())
    glDrawElements(GL_TRIANGLES, GetNrOfIndices(), GL_UNSIGNED_INT, 0);
  else
    glDrawArrays(GL_TRIANGLES, 0, GetNrOfVertices());
#pragma warning( pop )

  glBindVertexArray(0);
}

const bool Primitive::Empty() const
{
  return _vertices.size() == 0;
}

void Primitive::initGlData()
{
  //VAO VBO EBO, Generation of
  glGenVertexArrays(1, &_VAO);
  glBindVertexArray(_VAO);

  glGenBuffers(1, &_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, _VBO);
  glBufferData(GL_ARRAY_BUFFER, GetNrOfVertices() * sizeof(Vertex), GetVertices(), GL_STATIC_DRAW);

  if (GetNrOfIndices() > 0)
  {
    glGenBuffers(1, &_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetNrOfIndices() * sizeof(GLuint), GetIndices(), GL_STATIC_DRAW);
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

void Primitive::clearGlData()
{
  glDeleteBuffers(1, &_VBO);
  glDeleteBuffers(1, &_EBO);
  glDeleteVertexArrays(1, &_VAO);
}

coreone::Primitive::~Primitive()
{
  if (!Empty())
  {
    clearGlData();
  }
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

Pyramid::Pyramid()
{
  Vertex vertices[] = {
    //position                     //color                    //uvcoords               //normals                 
    glm::vec3(0.0f, 0.5f, 0.0f),   glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.5f, 1.f),     glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f),      glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(0.5f, -0.5f, 0.5f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),      glm::vec3(0.f, 0.f, 1.f),

    //position                     //color                    //uvcoords               //normals                 
    glm::vec3(0.0f, 0.5f, 0.0f),    glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.5f, 1.f),     glm::vec3(-1.f, 0.f, 0.f),
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f),      glm::vec3(-1.f, 0.f, 0.f),
    glm::vec3(-0.5f, -0.5f, 0.5f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),      glm::vec3(-1.f, 0.f, 0.f),

    //position                     //color                    //uvcoords               //normals                 
    glm::vec3(0.0f, 0.5f, 0.0f),  glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.5f, 1.f),      glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f),      glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),    glm::vec3(0.f, 0.f, -1.f),

    //position                     //color                    //uvcoords               //normals                 
    glm::vec3(0.0f, 0.5f, 0.0f),  glm::vec3(1.f, 1.f, 0.f), glm::vec2(0.5, 1.f),      glm::vec3(1.f, 0.f, 0.f),
    glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f),      glm::vec3(1.f, 0.f, 0.f),
    glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f),      glm::vec3(1.f, 0.f, 0.f)
  };
  unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

  unsigned nrOfIndices = 0;

  Set(vertices, nrOfVertices, nullptr, nrOfIndices);
}

Cube::Cube()
{
  Vertex vertices[] = {
    //back
    //position                       //color                      //uvcoords               //normals                 
    glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f),     glm::vec3(0.0f,  0.0f, -1.0f),
    glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f),     glm::vec3(0.0f,  0.0f, -1.0f),
    glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.f, 0.f, 1.f),  glm::vec2(1.f, 1.f),     glm::vec3(0.0f,  0.0f, -1.0f),
    //position                       //color                      //uvcoords               //normals                 
    glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 1.f),     glm::vec3(0.0f,  0.0f, -1.0f),
    glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec3(0.f, 1.f, 0.f),   glm::vec2(0.f, 1.f),     glm::vec3(0.0f,  0.0f, -1.0f),
    glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec3(1.f, 0.f, 0.f),  glm::vec2(0.f, 0.f),     glm::vec3(0.0f,  0.0f, -1.0f),
 
    //front
    //position                     //color                      //uvcoords              //normals                 
    glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f),     glm::vec3(0.0f,  0.0f, 1.0f),
    glm::vec3(0.5f, -0.5f,  0.5f),   glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f),     glm::vec3(0.0f,  0.0f, 1.0f),
    glm::vec3(0.5f,  0.5f,  0.5f),   glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 1.f),     glm::vec3(0.0f,  0.0f, 1.0f),
    //position                     //color                      //uvcoords              //normals                 
    glm::vec3(0.5f, 0.5f,  0.5f),  glm::vec3(0.f, 0.f, 1.f),  glm::vec2(1.f, 1.f),     glm::vec3(0.0f,  0.0f, 1.0f),
    glm::vec3(-0.5f, 0.5f,  0.5f), glm::vec3(0.f, 1.f, 0.f),  glm::vec2(0.f, 1.f),     glm::vec3(0.0f,  0.0f, 1.0f),
    glm::vec3(-0.5f, -0.5f,  0.5f),glm::vec3(1.f, 0.f, 0.f),  glm::vec2(0.f, 0.f),     glm::vec3(0.0f,  0.0f, 1.0f),

    //left
    //position                     //color                       //uvcoords              //normals               
    glm::vec3(-0.5f, -0.5f, -0.5f),    glm::vec3(1.f, 0.f, 0.f), glm::vec2(0.f, 0.f),     glm::vec3(-1.0f,  0.0f, 0.0f),
    glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.f, 1.f, 0.f),    glm::vec2(1.f, 0.f),       glm::vec3(-1.0f,  0.0f, 0.0f),
    glm::vec3(-0.5f, 0.5f,  0.5f),  glm::vec3(0.f, 0.f, 1.f),    glm::vec2(1.f, 1.f),     glm::vec3(-1.0f,  0.0f, 0.0f),
    //position                     //color                       //uvcoords              //normals                 
    glm::vec3(-0.5f, 0.5f,  0.5f),   glm::vec3(0.f, 0.f, 1.f),   glm::vec2(1.f, 1.f),    glm::vec3(-1.0f,  0.0f, 0.0f),
    glm::vec3(-0.5f, 0.5f,  -0.5f),   glm::vec3(0.f, 1.f, 0.f),  glm::vec2(0.f, 1.f),     glm::vec3(-1.0f,  0.0f, 0.0f),
    glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(1.f, 0.f, 0.f),   glm::vec2(0.f, 0.f),      glm::vec3(-1.0f,  0.0f, 0.0f),


    //right
    //position                     //color                      //uvcoords              //normals                 
    glm::vec3(0.5f, -0.5f,  0.5f),   glm::vec3(1.f, 0.f, 0.f),   glm::vec2(0.f, 0.f),    glm::vec3(1.0f,  0.0f, 0.0f),
    glm::vec3(0.5f, -0.5f, -0.5f),   glm::vec3(0.f, 1.f, 0.f),  glm::vec2(1.f, 0.f),   glm::vec3(1.0f,  0.0f, 0.0f),
    glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.f, 0.f, 1.f),   glm::vec2(1.f, 1.f),    glm::vec3(1.0f,  0.0f, 0.0f),
    //position                     //color                      //uvcoords              //normals              
    glm::vec3(0.5f,  0.5f, -0.5f),    glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 1.f),  glm::vec3(1.0f,  0.0f, 0.0f),
    glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.f, 1.f, 0.f),    glm::vec2(0.f, 1.f),     glm::vec3(1.0f,  0.0f, 0.0f),
    glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(1.f, 0.f, 0.f),    glm::vec2(0.f, 0.f),     glm::vec3(1.0f,  0.0f, 0.0f),

    //bottom
    //position                     //color                        //uvcoords              //normals                 
    glm::vec3(-0.5f, -0.5f, -0.5f),   glm::vec3(1.f, 0.f, 0.f),  glm::vec2(0.f, 0.f),    glm::vec3(0.0f,  -1.0f, 0.0f),
    glm::vec3( 0.5f, -0.5f, -0.5f),   glm::vec3(0.f, 1.f, 0.f),   glm::vec2(1.f, 0.f),   glm::vec3(0.0f,  -1.0f, 0.0f),
    glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec3(0.f, 0.f, 1.f),      glm::vec2(1.f, 1.f),    glm::vec3(0.0f,  -1.0f, 0.0f),
    //position                     //color                        //uvcoords              //normals            
    glm::vec3( 0.5f, -0.5f,  0.5f),    glm::vec3(0.f, 0.f, 1.f),    glm::vec2(1.f, 1.f),  glm::vec3(0.0f,  -1.0f, 0.0f),
    glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.f, 1.f, 0.f),     glm::vec2(0.f, 1.f),     glm::vec3(0.0f,  -1.0f, 0.0f),
    glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(1.f, 0.f, 0.f),   glm::vec2(0.f, 0.f),     glm::vec3(0.0f,  -1.0f, 0.0f),

    //top
    //position                     //color                       //uvcoords              //normals                 
    glm::vec3(-0.5f, 0.5f,  0.5f),   glm::vec3(1.f, 0.f, 0.f),  glm::vec2(0.f, 0.f),    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3( 0.5f, 0.5f,  0.5f),   glm::vec3(0.f, 1.f, 0.f),   glm::vec2(1.f, 0.f),     glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3( 0.5f, 0.5f, -0.5f),  glm::vec3(0.f, 0.f, 1.f),      glm::vec2(1.f, 1.f),        glm::vec3(0.0f, 1.0f, 0.0f),
    //position                     //color                       //uvcoords              //normals          
    glm::vec3( 0.5f, 0.5f, -0.5f),    glm::vec3(0.f, 0.f, 1.f),    glm::vec2(1.f, 1.f),      glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.f, 1.f, 0.f),     glm::vec2(0.f, 1.f),       glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(-0.5f, 0.5f,  0.5f),  glm::vec3(1.f, 0.f, 1.f),   glm::vec2(0.f, 0.f),     glm::vec3(0.0f, 1.0f, 0.0f)
  };
  unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

  unsigned nrOfIndices = 0;

  Set(vertices, nrOfVertices, nullptr, nrOfIndices);
}
