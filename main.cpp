#include <iostream>
#include "Generator.hpp"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: ssg build" << std::endl;
    return 1;
  }

  if (std::string(argv[1]) == "build") {
    return 0;
  }
}
