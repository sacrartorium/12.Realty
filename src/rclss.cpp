#include "fun.hpp"

void classificate(std::string &fullFileName, std::string &fileName,
                  std::ifstream &file) {
  dlib::one_vs_one_decision_function<Trainer, dlib::decision_function<Kernel>>
      classificator;
  dlib::deserialize(fullFileName) >> classificator;

  Unit unit;
  std::string line;
  while (std::getline(std::cin, line)) {
    try {
      unit = parse(line);

      long cluster = std::round(classificator(unit));

      fullFileName = fileName + ".c" + std::to_string(cluster);
      file.open(fullFileName);
    } catch (std::exception &e) {
      std::cout << "Wrong data! " << e.what() << std::endl;
      continue;
    }

    if (!file.good()) {
      std::cout << "Error open file!";
      continue;
    }

    Unit unit;
    std::multimap<double, Unit> clusters;

    while (std::getline(file, line)) {
      try {
        unit = parse(line);
      } catch (std::exception &e) {
        std::cout << "Error read file: " << e.what() << std::endl;
      };

      clusters.insert({distance(unit, unit), unit});
    }
    file.close();

    for (auto &cluster : clusters) {
      std::cout << cluster.second;
    }
  };
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Error: rclst <modelName>" << std::endl;
    return -1;
  }

  std::string fileName = argv[1];
  std::string fullFileName = fileName + ".dat";
  std::ifstream modelFile;

  // Test open model file
  {
    modelFile.open(fullFileName);
    if (!modelFile.good()) {
      std::cout << "Error open file!" << std::endl;
      return -1;
    }
    modelFile.close();
  }

  classificate(fullFileName, fileName, modelFile);

  return 0;
}
