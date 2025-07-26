#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <maddy/parser.h>
#include "FileManager.hpp"
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/node/node.h>

#define DEFAULT_OUT_PATH "./dist"

class Generator {
private:
  std::vector<std::string> expected_paths;
  FileManager fm;
  static std::optional<YAML::Node> extractAndRemoveMetadata(Markdown& md);
public:
  Generator(std::string src_path, std::optional<std::string> out_path = std::nullopt);
  static std::string convertToHtml(Markdown &md);
};

#endif // GENERATOR_H,
