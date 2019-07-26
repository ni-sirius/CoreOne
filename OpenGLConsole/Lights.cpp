#include "stdafx.h"
#include "lights/Lights.h"


using namespace light;

LightContainer::LightContainer(std::string id):
  _lightID(id),
  _containerNumber(-1)
{
}

std::string LightContainer::GetId()
{
  return _lightID;
}

int LightContainer::GetContainerNum()
{
  return _containerNumber;
}

void LightContainer::SetContainerNum(int num)
{
  _containerNumber = num;
}

LightContainer::LightProp& LightContainer::GetPropByName(std::string name)
{
  return _lightProperties[name];
}

std::unordered_map<std::string, LightContainer::LightProp> LightContainer::Properties()
{
  return _lightProperties;
}

void light::LightContainer::AddProperty(std::string id, LightProp prop)
{
  auto propPair = std::make_pair( id, prop );

  _lightProperties.insert(propPair);
}

void light::LightContainer::RemoveProperty(std::string id)
{
  _lightProperties.erase(id);
}
