#pragma once
#include <stdafx.h>

std::string loadShaderSource(std::string filename, const int versionMaj = 0, const int versionMin = 0)
{
  std::string src = "";
  std::ifstream in_file;

  in_file.open(filename);
  if (in_file.is_open())
  {
    std::string temp = "";
    while (std::getline(in_file, temp))
      src += temp + "\n";
  }
  else
  {
    std::cout << "ERROR::SHADER could not open file: " << filename << std::endl;
  }

  in_file.close();

  if (versionMaj != 0 && versionMin != 0)
  {
    std::string versionNum(std::to_string(versionMaj) + std::to_string(versionMin) + "0");
    src.replace(src.find("version"), 12, "version " + versionNum);
  }

  return src;
}