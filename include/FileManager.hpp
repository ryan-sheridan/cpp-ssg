#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>
#include "Markdown.hpp"

// can be file or dir
class FileNode {
public:
  std::filesystem::path path; // has a path
  bool is_directory;
  std::optional<Markdown> markdown_content; // if its a file
  std::vector<FileNode*> children; // for directorys
  std::optional<std::string> dist_root;

  // replaces the top level dir with the new root (used for out path)
  std::string replaceTopLevelDir(const std::string& new_root);

  // constructor for directory node
  FileNode(const std::filesystem::path& p, bool is_dir)
    : path(p), is_directory(is_dir) {}

  // constructor for file node
  FileNode(const std::filesystem::path& p, bool is_dir, Markdown content)
    : path(p), is_directory(is_dir), markdown_content(content) {}

  // define in cpp, deconstructor
  ~FileNode();
};

class FileStructureTree {
private:
  std::unique_ptr<FileNode> root_node;
  // oul helper
  void buildTreeRecursive(FileNode* current_node, const std::filesystem::path& current_path);
public:
  // build tree from base path
  FileStructureTree(const std::filesystem::path& base_path);
  FileNode *getRoot() const;
  void traverseAndPrint(FileNode* node, int depth = 0) const;
  void traverseConvertAndBuildDist(FileNode *node, std::filesystem::path out_path, int depth = 0) const;
};

class FileManager {
private:
  std::optional<std::filesystem::path> src_path;
  std::vector<std::string> root_paths;
public:
  FileManager(std::vector<std::string> expected_root_paths);
  // const tells compiler this function will not modify any member variables of the class
  bool isBasePathSet() const;

  // sets the basePath we need to work with
  void setBasePath(const std::filesystem::path& path);

  // validates the src file structure of basePath to make sure it matches a blog site
  // check for specific directorys, posts, assets
  void validateFileStructure();

  // read markdown file
  std::unique_ptr<FileStructureTree> readFiles();
};

#endif // FILEMANAGER_H
