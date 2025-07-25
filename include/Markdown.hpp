#ifndef MARKDOWN_H
#define MARKDOWN_H

#include <iostream>
#include <filesystem>

class Markdown {
public:
  std::filesystem::path file_path;
  std::string content;

  Markdown(const std::filesystem::path &path, const std::string &md_content)
      : file_path(path), content(md_content) {}

  // parse md, extract metadeta, etc
};

#endif
