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

namespace coreone::graphics
{
  class Shader
  {
  public:
    Shader(std::string vertexSource = "", std::string fragmenSource = "", std::string geometrySource = "");
    ~Shader();

    bool MakeProgram();

    void Use();
    void Unuse();

    //Temp new version,TODO create a uniform class later
    //Change order name first - value - if not scalar index
    //TODO Add array uniforms
    void SetUniform(const GLchar* name, GLfloat value);

    void SetUniform(const GLchar* name, GLint value);

    void SetUniform(const GLchar* name, glm::fvec2 value);
    void SetUniform(const GLchar* name, glm::fvec3 value);
    void SetUniform(const GLchar* name, glm::fvec4 value);

    void SetUniform(const GLchar* name, glm::mat3 value, GLboolean transpose = GL_FALSE);
    void SetUniform(const GLchar* name, glm::mat4 value, GLboolean transpose = GL_FALSE);

  private:
    GLuint compileShader(GLenum type, std::string source);
    GLuint linkProgram(GLuint vertexShader, GLuint geometryShader, GLuint fragmentShader);

  private:
    GLuint _programId;

    std::string _vertexSource;
    std::string _fragmentSource;
    std::string _geometrySource;

    //TODO add tess sources?

    int _versionMajor;
    int _versionMinor;
  };
} //namespace coreone::graphics