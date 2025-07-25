#ifndef GENERATOR_H
#define GENERATOR_H

#include <filesystem>
#include <iostream>
#include <maddy/parser.h>

class Markdown {
public:
  std::filesystem::path file_path;
  std::string content;

  Markdown(const std::filesystem::path &path, const std::string &md_content)
      : file_path(path), content(md_content) {}

  // parse md, extract metadeta, etc
};

#endif // GENERATOR_H,
