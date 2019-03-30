#pragma once
#include "CoreNode.h"
#include <map>

struct Character {
  GLuint     TextureID;  // ID handle of the glyph texture
  glm::ivec2 Size;       // Size of glyph
  glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
  GLuint     Advance;    // Offset to advance to next glyph
};


class TextNode : public CoreNode
{
public:
  TextNode(std::string text,
           glm::vec3 position,
           float scale,
           std::shared_ptr<Material> material = nullptr,
           bool visible = true);
  virtual ~TextNode();

  virtual void Update(const float& deltaTime, glm::mat4 modelMatrix = glm::mat4(1.f)) override;
  virtual void Render(glm::mat4 viewMat, glm::mat4 projectionMat,
    std::shared_ptr<Camera> camera,
    std::vector<std::shared_ptr<PointLight>> pointLights) override;

  inline void SetVisible(bool visible) { _visible = visible; };

  inline void SetText(const std::string text) { _text = text; };

  inline void SetPosition(const glm::vec3 position) { _position = position; };
  //inline void SetRotation(const glm::vec3 rotation) { _rotation = rotation; };
  inline void SetScale(const float scale) { _scale = scale; };

  inline void Move(const glm::vec3 position) { _position += position; };
  //inline void Rotate(const glm::vec3 rotation) { _rotation += rotation; };
  inline void Upscale(const float upscale) { _scale += upscale; };

  inline void SetMaterial(std::shared_ptr<Material> material) { _material = material; }

  //inline void SetDiffuseTexture(std::shared_ptr<Texture> texture) { _diffuseTexture = texture; }
  //inline void SetSpecularTexture(std::shared_ptr<Texture> texture) { _specularTexture = texture; }

protected:
  void initVAO();

  void updateModelMatrix();

protected:
  bool _visible;

  std::string _text;

  std::map<GLchar, Character> _characters;

  GLuint _VAO;
  GLuint _VBO;

  glm::vec3 _position;
  float _scale;

  std::shared_ptr<Material> _material;
};

