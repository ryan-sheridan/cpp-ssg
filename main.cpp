#include "FileManager.hpp"
#include "Generator.hpp"
#include <iostream>
#include <fstream>
#include <memory>

int main(int argc, char *argv[]) {
  //   if (argc < 2) {
  //     std::cerr << "Usage: ssg build" << std::endl;
  //     return 1;
  //   }
  //
  //   if (std::string(argv[1]) == "build") {
  //     return 0;
  //   }

  std::vector<std::string> expected_paths;

  // we expect all of these paths for a blog site
  expected_paths.push_back("posts");
  expected_paths.push_back("pages");
  expected_paths.push_back("assets");

  FileManager fm(expected_paths);

  return 0;
}
