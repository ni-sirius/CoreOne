#include "stdafx.h"
#include "Shader.h"


Shader::Shader(const int versionMaj, const int versionMin,
              std::string vertexFile /*= ""*/, std::string fragmentFile /*= ""*/, std::string geometryFile /*= ""*/):
  _versionMajor(versionMaj),
  _versionMinor(versionMin)
{
  GLuint vertexShader = vertexFile.empty() ? 0 : loadShader(GL_VERTEX_SHADER, vertexFile);
  GLuint geometryShader = geometryFile.empty() ? 0 : loadShader(GL_GEOMETRY_SHADER, geometryFile);
  GLuint fragmentShader = fragmentFile.empty() ? 0 : loadShader(GL_FRAGMENT_SHADER, fragmentFile);

  this->linkProgram(vertexShader, geometryShader, fragmentShader);

  //End
  glDeleteShader(vertexShader);
  glDeleteShader(geometryShader);
  glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
  glDeleteProgram(this->_id);
}

void Shader::Use()
{
  glUseProgram(this->_id);
}

void Shader::Unuse()
{
  glUseProgram(0);
}

void Shader::SetUniform(const GLchar* name, GLfloat value)
{
  this->Use();

  glUniform1f(glGetUniformLocation(this->_id, name), value);

  this->Unuse();
}

void Shader::SetUniform(const GLchar* name, GLint value)
{
  this->Use();

  glUniform1i(glGetUniformLocation(this->_id, name), value);

  this->Unuse();
}

void Shader::SetUniform(const GLchar* name, glm::fvec2 value)
{
  this->Use();

  glUniform2fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));

  this->Unuse();
}

void Shader::SetUniform(const GLchar* name, glm::fvec3 value)
{
  this->Use();

  glUniform3fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));

  this->Unuse();
}

void Shader::SetUniform(const GLchar* name, glm::fvec4 value)
{
  this->Use();

  glUniform4fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));

  this->Unuse();
}

void Shader::SetUniform(const GLchar* name, glm::mat3 value, GLboolean transpose /*= GL_FALSE*/)
{
  this->Use();

  glUniformMatrix3fv(glGetUniformLocation(this->_id, name), 1, transpose, glm::value_ptr(value));

  this->Unuse();
}

void Shader::SetUniform(const GLchar* name, glm::mat4 value, GLboolean transpose /*= GL_FALSE*/)
{
  this->Use();

  glUniformMatrix4fv(glGetUniformLocation(this->_id, name), 1, transpose, glm::value_ptr(value));

  this->Unuse();
}


//OLD(Don't use)
void Shader::Set1f(GLfloat value, const GLchar* name)
{
  this->Use();

  glUniform1f(glGetUniformLocation(this->_id, name), value);

  this->Unuse();
}

void Shader::SetVec2f(glm::fvec2 value, const GLchar* name)
{
  this->Use();

  glUniform2fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));

  this->Unuse();
}

void Shader::SetVec3f(glm::fvec3 value, const GLchar* name)
{
  this->Use();

  glUniform3fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));

  this->Unuse();
}

void Shader::SetVec4f(glm::fvec4 value, const GLchar* name)
{
  this->Use();

  glUniform4fv(glGetUniformLocation(this->_id, name), 1, glm::value_ptr(value));

  this->Unuse();
}

void Shader::SetMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose /*= GL_FALSE*/)
{
  this->Use();

  glUniformMatrix3fv(glGetUniformLocation(this->_id, name), 1, transpose, glm::value_ptr(value));

  this->Unuse();
}

void Shader::SetMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose /*= GL_FALSE*/)
{
  this->Use();

  glUniformMatrix4fv(glGetUniformLocation(this->_id, name), 1, transpose, glm::value_ptr(value));

  this->Unuse();
}

void Shader::Set1i(GLint value, const GLchar* name)
{
  this->Use();

  glUniform1i(glGetUniformLocation(this->_id, name), value);

  this->Unuse();
}

std::string Shader::loadShaderSource(std::string filename)
{
  std::string temp = "";
  std::string src = "";

  std::ifstream in_file;

  //Vertex
  in_file.open(filename);
  if (in_file.is_open())
  {
    while (std::getline(in_file, temp))
      src += temp + "\n";
  }
  else
  {
    std::cout << "ERROR::SHADER could not open file: "<< filename << std::endl;
  }

  in_file.close();

  std::string versionNum(std::to_string(_versionMajor) + std::to_string(_versionMinor) + "0");
  src.replace(src.find("version"), 12, "version " + versionNum);

  return src;
}

GLuint Shader::loadShader(GLenum type, std::string filename)
{
  char infoLog[512];
  GLint success;

  GLuint shader = glCreateShader(type);
  std::string srcText = this->loadShaderSource(filename);
  const GLchar* src = srcText.c_str();
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);

    std::cout << "ERROR::SHADER could not compile shader file "<< filename << std::endl;
    std::cout << infoLog << std::endl;
  }

  return shader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader)
{
  char infoLog[512];
  GLint success;

  this->_id = glCreateProgram();
  glUseProgram(this->_id); //TODO

  if (vertexShader)
    glAttachShader(this->_id, vertexShader);

  if (geometryShader)
    glAttachShader(this->_id, geometryShader);

  if (fragmentShader)
    glAttachShader(this->_id, fragmentShader);

  glLinkProgram(this->_id);

  glGetProgramiv(this->_id, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(this->_id, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER Could not link shader program" << std::endl;
    std::cout << infoLog << std::endl;
  }

  glUseProgram(0);
}