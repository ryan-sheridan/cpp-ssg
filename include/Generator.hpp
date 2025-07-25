#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <maddy/parser.h>
#include "FileManager.hpp"

#define DEFAULT_OUT_PATH "./dist"

class Generator {
private:
  std::vector<std::string> expected_paths;
  FileManager fm;
public:
  Generator(std::string src_path, std::optional<std::string> out_path = nullptr);
};

#endif // GENERATOR_H,
