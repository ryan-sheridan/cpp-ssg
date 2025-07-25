#include "FileManager.hpp"
#include "Generator.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <stdexcept>

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

  try {
    std::filesystem::create_directories("blog_src/posts");
    std::filesystem::create_directories("blog_src/pages");
    std::filesystem::create_directories("blog_src/assets");

    std::ofstream("blog_src/index.md") << "# welcome\nthis is my homepage!";
    std::ofstream("blog_src/posts/first_post.md") << "## my first post\nsome content.";
    std::ofstream("blog_src/posts/second_post.md") << "### another post\nmoreeee content.";
    std::ofstream("blog_src/pages/about.md") << "# about \nlearn more.";
    std::ofstream("blog_src/assets/image.jpg") << "binary image data (not actually read by markdown)";

    fm.setBasePath("blog_src");
    fm.validateFileStructure();

    std::cout << "\nreading files and building tree..." << std::endl;
    std::unique_ptr<FileStructureTree> tree = fm.readFiles();

    std::cout << "\ntraversing the file structure tree:" << std::endl;
    if (tree) {
      tree->traverseAndPrint(tree->getRoot());
    }
  } catch(const std::runtime_error& e) {
    std::cerr << "err: " << e.what() << std::endl;
    std::filesystem::remove_all("blog_src");
  }

  return 0;
}
