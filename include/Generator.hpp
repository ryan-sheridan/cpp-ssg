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

typedef std::string HTML;

typedef struct {
  std::optional<YAML::Node> metadata;
  HTML content;
} HtmlDocument;

class Generator {
private:
  std::vector<std::string> expected_paths;
  FileManager fm;
  static std::optional<YAML::Node> extractAndRemoveMetadata(Markdown& md);
public:
  Generator(std::string src_path, std::optional<std::string> out_path = std::nullopt);
  static HtmlDocument convertToHtml(Markdown &md);
  static HTML transformDocumentWithTemplate(std::filesystem::path template_path, HtmlDocument hd);
};

#endif // GENERATOR_H,
