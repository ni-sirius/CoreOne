#include "stdafx.h"
#include <graphics/Shader.h>

using namespace coreone::graphics;

Shader::Shader(std::string vertexSource /*= ""*/,
    std::string fragmenSource /*= ""*/,
    std::string geometrySource /*= ""*/)
  : _vertexSource(vertexSource)
  , _fragmentSource(fragmenSource)
  , _geometrySource(geometrySource)
{
}

Shader::~Shader()
{
  glDeleteProgram(_programId);
}

bool Shader::MakeProgram()
{
  GLuint vertexShader = _vertexSource.empty() ? 0 : compileShader(GL_VERTEX_SHADER, _vertexSource);
  GLuint fragmentShader = _fragmentSource.empty() ? 0 : compileShader(GL_FRAGMENT_SHADER, _fragmentSource);
  GLuint geometryShader = _geometrySource.empty() ? 0 : compileShader(GL_GEOMETRY_SHADER, _geometrySource);

  return linkProgram(vertexShader, geometryShader, fragmentShader);
}

void Shader::Use()
{
  glUseProgram(_programId);
}

void Shader::Unuse()
{
  glUseProgram(0);
}

void Shader::SetUniform(const GLchar* name, GLfloat value)
{
  Use();

  glUniform1f(glGetUniformLocation(_programId, name), value);

  Unuse();
}

void Shader::SetUniform(const GLchar* name, GLint value)
{
  Use();

  glUniform1i(glGetUniformLocation(_programId, name), value);

  Unuse();
}

void Shader::SetUniform(const GLchar* name, glm::fvec2 value)
{
  Use();

  glUniform2fv(glGetUniformLocation(_programId, name), 1, glm::value_ptr(value));

  Unuse();
}

void Shader::SetUniform(const GLchar* name, glm::fvec3 value)
{
  Use();

  glUniform3fv(glGetUniformLocation(_programId, name), 1, glm::value_ptr(value));

  Unuse();
}

void Shader::SetUniform(const GLchar* name, glm::fvec4 value)
{
  Use();

  glUniform4fv(glGetUniformLocation(_programId, name), 1, glm::value_ptr(value));

  Unuse();
}

void Shader::SetUniform(const GLchar* name, glm::mat3 value, GLboolean transpose /*= GL_FALSE*/)
{
  Use();

  glUniformMatrix3fv(glGetUniformLocation(_programId, name), 1, transpose, glm::value_ptr(value));

  Unuse();
}

void Shader::SetUniform(const GLchar* name, glm::mat4 value, GLboolean transpose /*= GL_FALSE*/)
{
  Use();

  glUniformMatrix4fv(glGetUniformLocation(_programId, name), 1, transpose, glm::value_ptr(value));

  Unuse();
}

GLuint Shader::compileShader(GLenum type, std::string source)
{
  std::string log;
  GLint success;

  GLuint shader = glCreateShader(type);
  const GLchar* src = source.c_str();
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, log.data());

    std::cout << "ERROR::SHADER could not compile shader " << type << std::endl;
    std::cout << log << std::endl;

    glDeleteShader(shader);

    return 0;
  }

  return shader;
}

GLuint Shader::linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
{
  if (!vertexShader && !geometryShader && !fragmentShader)
  {
    return 0;
  }

  std::string log;
  GLint success;

  _programId = glCreateProgram();
  if (!_programId)
    return 0;

  glUseProgram(_programId);

  if (vertexShader)
    glAttachShader(_programId, vertexShader);

  if (geometryShader)
    glAttachShader(_programId, geometryShader);

  if (fragmentShader)
    glAttachShader(_programId, fragmentShader);

  glLinkProgram(_programId);

  glGetProgramiv(_programId, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(_programId, 512, NULL, log.data());
    std::cout << "ERROR::SHADER Could not link shader program" << std::endl;
    std::cout << log << std::endl;

    glDeleteProgram(_programId);
    _programId = 0;
  }

  glUseProgram(0);

  glDeleteShader(vertexShader);
  glDeleteShader(geometryShader);
  glDeleteShader(fragmentShader);

  return _programId;
}