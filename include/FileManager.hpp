#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <filesystem>
#include <optional>
#include "Generator.hpp"

// can be file or dir
class FileNode {
public:
  std::filesystem::path path; // has a path
  bool is_directory;
  std::optional<Markdown> markdown_content; // if its a file
  std::vector<FileNode*> children; // for directorys

  // constructor for directory node
  FileNode(const std::filesystem::path& p, bool is_dir)
    : path(p), is_directory(is_dir) {}

  // constructor for file node
  FileNode(const std::filesystem::path& p, bool is_dir, Markdown content)
    : path(p), is_directory(is_dir), markdown_content(content) {}

  // define in cpp, deconstructor
  ~FileNode();
};

class FileManager {
private:
  std::optional<std::filesystem::path> src_path;
  std::vector<Markdown> src;
public:
  // validates the src file structure of basePath to make sure it matches a blog site
  void validateFileStructure();

  // const tells compiler this function will not modify any member variables of the class
  bool isBasePathSet() const;

  // sets the basePath we need to work with
  void setBasePath(const std::filesystem::path& path);

  // read markdown file
  Markdown readFile();
};

#endif // FILEMANAGER_H
