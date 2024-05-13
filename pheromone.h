/**
 * @author: Rafał Skinderowicz (rafal.skinderowicz@us.edu.pl)
*/
#pragma once

#include <algorithm>
#include <cstdint>
#include <vector>
#include <cassert>

#include "utils.h"
#include <iostream>
#include "progargs.h"

struct MatrixPheromone {
    uint32_t dimension_ = 0;
    std::vector<double> trails; // For every edge (a,b),
                                 // where 0 <= a, b < dimension_
    double rho;
    double tau_min, tau_max;


    double get(uint32_t node) const {
        assert(node < dimension_);
        return trails[node];
    }

    void init_smooth(int dimension, const ProgramOptions &options) {
        dimension_ = dimension;
        rho = options.rho_;
        tau_max = options.tau_max_;
        tau_min = options.tau_min_;
        for (int i = 0; i < dimension_; i++) {
            trails.push_back(tau_max);
        }
    }

    void evaporate_smooth(int node) {
        assert(node < dimension_);
        trails[node] = trails[node] * (1 - rho)  + rho * tau_min;
    }

    void increase_smooth(uint32_t node) {
        assert(node < dimension_);
        trails[node] = trails[node] * (1 - rho) + rho * tau_max;
    }
};