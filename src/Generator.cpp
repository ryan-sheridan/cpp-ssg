#include "Generator.hpp"
#include "FileManager.hpp"

Generator::Generator(std::string src_path, std::optional<std::string> out_path)
  : expected_paths{"posts", "pages", "assets"},
    fm(expected_paths)
{
  try {
    fm.setBasePath(src_path);
    fm.validateFileStructure();

    std::cout << "\nreading files and building tree..." << std::endl;
    std::unique_ptr<FileStructureTree> tree = fm.readFiles();

    std::cout << "\ntraversing the file structure tree:" << std::endl;
    if (tree) {
      tree->traverseAndPrint(tree->getRoot());
      if(out_path.has_value()) {
        tree->traverseAndBuildDist(tree->getRoot(), out_path.value_or(DEFAULT_OUT_PATH));
      }
    }
  } catch(const std::runtime_error& e) {
    std::cerr << "err: " << e.what() << std::endl;
  }
}

