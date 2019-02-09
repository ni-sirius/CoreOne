#pragma once
#include <string>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <mat4x4.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

class Shader
{
public:
  Shader(const int versionMaj, const int versionMin,
         std::string vertexFile = "", std::string fragmentFile = "", std::string geometryFile = "");
  ~Shader();

  void Use();
  void Unuse();

  void Set1f(GLfloat value, const GLchar* name);

  void Set1i(GLint value, const GLchar* name);

  void SetVec2f(glm::fvec2 value, const GLchar* name);
  void SetVec3f(glm::fvec3 value, const GLchar* name);
  void SetVec4f(glm::fvec4 value, const GLchar* name);

  void SetMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE);
  void SetMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE);

private:
  GLuint _id;
  int _versionMajor;
  int _versionMinor;

private:
  std::string loadShaderSource(std::string filename);

  GLuint loadShader(GLenum type, std::string filename);

  void linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);
};