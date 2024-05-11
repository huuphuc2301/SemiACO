/**
 * @author: Rafał Skinderowicz (rafal.skinderowicz@us.edu.pl)
 */
#pragma once

#include <string>

struct ProgramOptions {
    std::string algorithm_ = "faco";

    uint32_t ants_count_ = 100;

    std::string id_ = "default";  // Id of the comp. experiment

    int32_t iterations_ = 300;

    std::string problem_path_ = "kroA100.tsp";

    // By default the results will be stored in "results" folder
    std::string results_dir_ = "results";

    double rho_ = 0.4;
    double tau_min_ = 0.1;
    double tau_max_ = 0.5;
    double trail_min = 0.1;
    double trail_max = 1.0;

    int target_ = 100;

    double q0 = 0.3;
};