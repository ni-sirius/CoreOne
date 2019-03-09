#include "stdafx.h"
#include "Render.h"

#include "Core.h"

std::vector< std::shared_ptr<Material> > materials;
std::vector< std::shared_ptr<Texture> > textures;

void initMaterials()
{
  materials.push_back(
    std::make_shared<Material>(
      glm::vec3(0.1f), 32, 0, 1));

  materials.push_back(
    std::make_shared<Material>(
      glm::vec3(1.0f), 32, 0, 1));
}

void initTextures()
{
  textures.push_back(std::make_shared<Texture>("Images/apple.png", GL_TEXTURE_2D));
  textures.push_back(std::make_shared<Texture>("Images/apple_specular.png", GL_TEXTURE_2D));

  textures.push_back(std::make_shared<Texture>("Images/flower.png", GL_TEXTURE_2D));
  textures.push_back(std::make_shared<Texture>("Images/flower_specular.png", GL_TEXTURE_2D));

  textures.push_back(std::make_shared<Texture>("Images/container.png", GL_TEXTURE_2D));
  textures.push_back(std::make_shared<Texture>("Images/container_specular.png", GL_TEXTURE_2D));
}

void Create(Core& core)
{
  initMaterials();
  initTextures();

  //Lights
  auto lightNode = new LightNode(glm::vec3(2.f, 1.5f, 1.f), glm::vec3(1.f));
  core.AddLightSceneNode(lightNode);

  auto lightMesh = new MeshNode(std::make_shared<Cube>(), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(0.5f), materials[1]);
  core.AddMeshSceneNode(lightMesh, lightNode);

  //Meshes
  auto flowerBox = new MeshNode(std::make_shared<Cube>(), glm::vec3(-1.f, 1.f, 0.f), glm::vec3(0.f), glm::vec3(1.f), materials[0], textures[2], textures[3]);
  core.AddMeshSceneNode(flowerBox);

  auto container1 = new MeshNode(std::make_shared<Cube>(), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f), materials[0], textures[4], textures[5]);
  core.AddMeshSceneNode(container1);

  auto container2 = new MeshNode(std::make_shared<Cube>(), glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.f), glm::vec3(1.f), materials[0], textures[4], textures[5]);
  core.AddMeshSceneNode(container2, container1);

}

void RenderLoop()
{
  Core coreObject("OpenGL Test App", 1024, 768, 4, 5, false);

  Create(coreObject);

  //Main loop
  while (!coreObject.GetWindiwShouldClose())
  {
    //update input
    coreObject.Update();
    coreObject.Render();

    //Clear up all
    glBindVertexArray(0);
    glUseProgram(0);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
}