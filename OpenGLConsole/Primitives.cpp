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
