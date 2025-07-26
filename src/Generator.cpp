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

std::optional<YAML::Node> Generator::extractAndRemoveMetadata(Markdown& md) {
  // find the offset of the first ---
  size_t start = md.content.find("---");
  if (start == std::string::npos || start != 0) return std::nullopt;

  start += 3;
  // find the offset of the second ---
  size_t end = md.content.find("---", start);
  if (end == std::string::npos) return std::nullopt;

  // take everything inside of --- and put it into a string
  std::string yaml_str = md.content.substr(start, end - start);

  // trim newlines and stuff, makes are html better
  size_t first = yaml_str.find_first_not_of("\r\n");
  size_t last = yaml_str.find_last_not_of("\r\n");
  if (first != std::string::npos && last != std::string::npos)
    yaml_str = yaml_str.substr(first, last - first + 1);

  // remove the --- from the md, once again, better html
  size_t afterYamlPos = end + 3;
  while (afterYamlPos < md.content.size() && (md.content[afterYamlPos] == '\n' || md.content[afterYamlPos] == '\r'))
    ++afterYamlPos;
  md.content = md.content.substr(afterYamlPos);

  // parse into a YAML::Node
  try {
    std::istringstream yaml_ss(yaml_str);
    YAML::Node node = YAML::Load(yaml_ss);
    return node;
  } catch (const YAML::ParserException& e) {
    // yaml malformed
    std::cerr << "YAML parse error: " << e.what() << "\n";
    return std::nullopt;
  }
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

  std::optional<YAML::Node> metadata = extractAndRemoveMetadata(md);
  if(metadata.has_value()) {
    // spits out title if there is one
    std::cout << "title: " << (*metadata)["title"].as<std::string>() << "\n";
  }

  // convert to strstream
  std::stringstream content_ss(md.content);

  // return parsed html
  return parser->Parse(content_ss);
}
