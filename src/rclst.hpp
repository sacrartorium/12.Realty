#pragma once

#include "fun.hpp"

void save(std::vector<Unit> &units, std::vector<Cluster> &clusters,
          std::string fileName, Cluster countClusters) {
  std::vector<std::shared_ptr<std::ofstream>> files;
  files.reserve(countClusters);

  for (Cluster cluster = 0; cluster < countClusters; cluster++) {
    files.push_back(std::make_shared<std::ofstream>(fileName + ".c" +
                                                    std::to_string(cluster)));
  }

  for (uint64_t index = 0; index < units.size() && index < clusters.size();
       index++) {
    Cluster cluster = clusters[index];
    Unit unit = units[index];

    std::cout << cluster << ":" << unit;

    std::ofstream &file(*files[cluster]);
    file << unit;
  }
}
