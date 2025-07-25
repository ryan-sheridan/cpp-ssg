#include "FileManager.hpp"
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>

// deconstructor
FileNode::~FileNode() {
  for (FileNode *child : children) {
    delete child;
  }
}

// constructor to build graph from base path
FileStructureTree::FileStructureTree(const std::filesystem::path &base_path) {
  // if the base path does not exist or is not a directory
  if (!std::filesystem::exists(base_path) |
      !std::filesystem::is_directory(base_path)) {
    throw std::runtime_error("base_path does not exist or is not a directory");
  }
  // create root dir node
  root_node = std::make_unique<FileNode>(base_path, true);
  buildTreeRecursive(root_node.get(), base_path);
}

// return root node
FileNode *FileStructureTree::getRoot() const { return root_node.get(); }

void FileStructureTree::buildTreeRecursive(
    FileNode *current_node, const std::filesystem::path &current_path) {
  if (std::filesystem::is_directory(current_path)) {
    // for every file/directory
    for (const auto &entry :
         std::filesystem::directory_iterator(current_path)) {
      // if its a directory
      if (entry.is_directory()) {
        // create a file node
        FileNode *child_dir_node = new FileNode(entry.path(), true);
        // insert at the end of children vector
        current_node->children.push_back(child_dir_node);
        buildTreeRecursive(child_dir_node, entry.path());
      } else if (entry.is_regular_file()) { // or if its a file
        // we only care about markdown files
        if (entry.path().extension() == ".md") {
          // read file content
          std::ifstream file(entry.path()); // NOTE: remember fstream
          std::string content((std::istreambuf_iterator<char>(file)),
                              std::istreambuf_iterator<char>());
          Markdown md(entry.path(), content);
          FileNode *child_file_node = new FileNode(entry.path(), false, md);
          current_node->children.push_back(child_file_node);
        } else {
          // TODO: config files maybe
          FileNode *child_file_node = new FileNode(entry.path(), false);
          current_node->children.push_back(child_file_node);
        }
      }
    }
  }
}

void FileStructureTree::traverseAndPrint(FileNode *node, int depth) const {
  for (int i = 0; i < depth; i++) {
    std::cout << " ";
  }
  std::cout << (node->is_directory ? "[DIR] " : "[FILE] ");
  std::cout << node->path.filename().string();
  // if there is markdown content, print the size
  if (node->markdown_content.has_value()) {
    std::cout << "(MD content size: "
              << node->markdown_content->content.length() << ")";
  }
  std::cout << std::endl;
  // recursion and shit
  for (FileNode *child : node->children) {
    traverseAndPrint(child, depth + 1);
  }
}

void FileStructureTree::traverseAndBuildDist(FileNode *node, std::filesystem::path out_path, int depth) const {
  // create dist folder if does not exist
  if(!std::filesystem::is_directory(out_path) || !std::filesystem::exists(out_path)) {
    std::filesystem::create_directory(out_path);
  }



}

// FileManager

FileManager::FileManager(std::vector<std::string> expected_root_paths) {
  root_paths = expected_root_paths;
}

bool FileManager::isBasePathSet() const { return src_path.has_value(); }

void FileManager::setBasePath(const std::filesystem::path &path) {
  if (!std::filesystem::exists(path) | !std::filesystem::is_directory(path)) {
    throw std::runtime_error(
        "provided path does not exist or is not a directory");
  }
  src_path = path;
}

void FileManager::validateFileStructure() {
  // check base path is set
  if (!isBasePathSet()) {
    throw std::runtime_error("base path not set, call set base path first");
  }

  for (auto path : root_paths) {
    std::filesystem::path posts_dir = src_path.value() / path;
    if (!std::filesystem::exists(posts_dir) ||
        !std::filesystem::is_directory(posts_dir)) {
      throw std::runtime_error("invalid file structure: '" + path +
                               "' directory missing.");
    }
  }

  std::cout << "file structure validated for" << src_path->string()
            << std::endl;
}

std::unique_ptr<FileStructureTree> FileManager::readFiles() {
  if (!isBasePathSet()) {
    throw std::runtime_error("base path not set, please setBasePath first");
  }

  return std::make_unique<FileStructureTree>(src_path.value());
}
