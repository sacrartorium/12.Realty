#include "rclst.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "Error: rclst <countClusters> <modelName>" << std::endl;
    return -1;
  }

  Cluster countClusters = std::stoul(argv[1]);
  std::string modelName = argv[2];
  if (countClusters <= 1) {
    std::cout << "countClusters > 1!" << std::endl;
    return -1;
  }

  Unit unit;
  std::vector<Unit> units;
  std::string line;

  // Get data
  {
    std::getline(std::cin, line);

    while (!std::cin.eof()) {
      try {
        unit = parse(line);
        units.push_back(unit);

      } catch (std::exception &ex) {
        std::cout << "Error read file: " << ex.what() << std::endl;
      }
      std::getline(std::cin, line);
    };

    std::cout << units.size() << " units." << std::endl;
  }

  // Clusterization
  {
    std::cout << "Clusterization" << std::endl;
    dlib::kcentroid<Kernel> kc(Kernel(0.01), 0.01, countClusters);
    dlib::kkmeans<Kernel> kk(kc);
    std::vector<Unit> initialCenters;

    kk.set_number_of_centers(countClusters);
    dlib::pick_initial_centers(countClusters, initialCenters, units,
                               kk.get_kernel());
    kk.train(units, initialCenters, 1000);
  }

  std::vector<Cluster> assigns =
      spectral_cluster(Kernel(0.01), units, countClusters);

  save(units, assigns, modelName, countClusters);
  std::cout << "Clusters saved." << std::endl;

  {
    std::cout << "Training started." << std::endl;
    Trainer trainer;
    dlib::krr_trainer<Kernel> linearTrainer;
    linearTrainer.set_kernel(Kernel(0.01));
    trainer.set_trainer(linearTrainer);
    trainer.be_verbose();

    std::vector<double> labels;
    std::copy(assigns.begin(), assigns.end(), back_inserter(labels));
    dlib::one_vs_one_decision_function<Trainer, dlib::decision_function<Kernel>>
        function = trainer.train(units, labels);
    dlib::serialize(modelName + ".dat") << function;
  }

  std::cout << "Model saved in " << modelName << ".dat" << std::endl;
  return 0;
}
