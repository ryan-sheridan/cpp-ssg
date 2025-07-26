#include "FileManager.hpp"
#include "Generator.hpp"
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

std::string FileNode::replaceTopLevelDir(const std::string &new_root) {
  std::filesystem::path new_path = new_root;

  // skip first element, "eg. blog_src"
  auto it = path.begin();
  ++it;

  for (; it != path.end(); ++it) {
    // TODO: what the hell, how does this work with /
    new_path /= *it;
  }

  return new_path;
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

void FileStructureTree::traverseConvertAndBuildDist(
    FileNode *node, std::filesystem::path root_path, int depth) const {

  // create dist folder if does not exist
  if (!std::filesystem::is_directory(root_path) ||
      !std::filesystem::exists(root_path)) {
    std::filesystem::create_directory(root_path);
  }

  // create final out path for file or dir
  std::filesystem::path out_path = node->replaceTopLevelDir(root_path);

  // if its a directory, we create it
  if (node->is_directory) {
    std::filesystem::create_directory(out_path);
  } else if (node->markdown_content.has_value()) {
    // convert to html
    HtmlDocument hd =
      Generator::convertToHtml(node->markdown_content.value());

    std::string html_content = hd.content;

    // if the final out path is not a directory
    if (!std::filesystem::is_directory(out_path)) {
      // create parent directories if they don't exist
      std::filesystem::create_directories(out_path.parent_path());

      // replace .md with .html
      out_path.replace_extension(".html");

      // open/create file for writing
      std::ofstream o(out_path);
      if (!o) {
        throw std::runtime_error("failed to open '" +
                                 out_path.generic_string() + "' for writing");
      }
      // write to file
      o << html_content << std::endl;
    }
  }
  for (FileNode *child : node->children) {
    traverseConvertAndBuildDist(child, root_path, depth + 1);
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
