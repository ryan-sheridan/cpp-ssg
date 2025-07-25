#include "FileManager.hpp"

// deconstructor
FileNode::~FileNode() {
  for(FileNode* child : children) {
    delete child;
  }
}
