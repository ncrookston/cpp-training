
#include "image_loader.hpp"
#include "deserializer.hpp"
#include <fstream>

char* load_image(deserializer& deserializer, std::istream& in)
{ return deserializer.read(in); }

image_loader::operator std::unique_ptr<char[]>()
{
  std::ifstream in(m_name.c_str(), std::ios::binary);
  std::unique_ptr<char[]> pImg(load_image(*m_deserializer, in));
  //Um, assume we process the image somehow using our workers here. . .

  return pImg;
}

