#include "Generator.hpp"
#include "FileManager.hpp"

Generator::Generator(std::string src_path, std::optional<std::string> out_path)
    : expected_paths{"posts", "pages", "assets"}, fm(expected_paths) {
  try {
    fm.setBasePath(src_path);
    fm.validateFileStructure();

    std::cout << "\nreading files and building tree..." << std::endl;
    std::unique_ptr<FileStructureTree> tree = fm.readFiles();

    std::cout << "\ntraversing, converting, and building dist..." << std::endl;
    if (tree) {
      if (out_path.has_value()) {
        tree->traverseConvertAndBuildDist(tree->getRoot(),
                                          out_path.value_or(DEFAULT_OUT_PATH));
      }
    }
  } catch (const std::runtime_error &e) {
    std::cerr << "err: " << e.what() << std::endl;
  }
}

YAML::Node Generator::extractAndRemoveMetadata(Markdown& md) {

}

std::string Generator::convertToHtml(Markdown& md) {
  // config
  std::shared_ptr<maddy::ParserConfig> config =
      std::make_shared<maddy::ParserConfig>();
  // do not wrap in paragraph
  config->enabledParsers |= maddy::types::HTML_PARSER;
  // create parser
  std::shared_ptr<maddy::Parser> parser =
      std::make_shared<maddy::Parser>(config);

  auto metadata = extractAndRemoveMetadata(md);

  // convert to strstream
  std::stringstream content_ss(content);

  // return parsed html
  return parser->Parse(content_ss);
}
