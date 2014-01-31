#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#define QUALITY 19

#include "deserializer.hpp"
#include <memory>
#include <string>
#include <vector>

class worker {};

class image_loader
{
public:
  image_loader(const std::string& name, int quality = QUALITY)
  {
    m_deserializer.reset(new deserializer(quality));
    m_numWorkers = 5;
    m_workers.resize(m_numWorkers);
    m_name = name;
  }

  // Converts from an image_loader object to the loaded image.
  // Usage:
  //
  // image_loader loader;
  // boost::shared_array<char> pImg = loader; 
  operator std::unique_ptr<char[]>();

  int m_numWorkers;
  std::string m_name;
  std::shared_ptr<deserializer> m_deserializer;
  std::vector<worker> m_workers;

};//end image_loader

#endif
