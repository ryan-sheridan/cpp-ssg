#include "FileManager.hpp"
#include "Generator.hpp"
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>

void test() {
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
    std::ofstream("blog_src/posts/first_post.md")
        << "## my first post\nsome content.";
    std::ofstream("blog_src/posts/second_post.md")
        << "### another post\nmoreeee content.";
    std::ofstream("blog_src/pages/about.md") << "# about \nlearn more.";
    std::ofstream("blog_src/assets/image.jpg")
        << "binary image data (not actually read by markdown)";

    fm.setBasePath("blog_src");
    fm.validateFileStructure();

    std::cout << "\nreading files and building tree..." << std::endl;
    std::unique_ptr<FileStructureTree> tree = fm.readFiles();

    std::cout << "\ntraversing the file structure tree:" << std::endl;
    if (tree) {
      tree->traverseAndPrint(tree->getRoot());
    }
  } catch (const std::runtime_error &e) {
    std::cerr << "err: " << e.what() << std::endl;
    std::filesystem::remove_all("blog_src");
  }
}

int main(int argc, char* argv[]) {
  if (argc < 3 || std::string(argv[1]) != "build") {
    std::cerr << "Usage: ssg build <src_path> [-o output_path]" << std::endl;
    return 1;
  }

  std::string src_path = argv[2];
  std::optional<std::string> out_path;

  if (argc == 5 && std::string(argv[3]) == "-o") {
    out_path = argv[4];
  } else if (argc > 3) {
    std::cerr << "Invalid arguments.\nUsage: ssg build <src_path> [-o output_path]" << std::endl;
    return 1;
  }

  try {
    Generator gm(src_path, out_path);
  } catch (const std::runtime_error& e) {
    std::cerr << "error happened when creating generator: " << e.what() << std::endl;
  }

  return 0;
}
