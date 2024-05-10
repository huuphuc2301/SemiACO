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
    double trail_min, trail_max;


    double get(uint32_t node) const {
        assert(node < dimension_);
        return trails[node];
    }

    void init_smooth(int dimension, const ProgramOptions &options) {
        dimension_ = dimension;
        rho = options.rho_;
        tau_max = options.tau_max_;
        tau_min = options.tau_min_;
        trail_min = options.trail_min;
        trail_max = options.trail_max;
        for (int i = 0; i < dimension_; i++) {
            trails.push_back(trail_max);
        }
    }

    void evaporate_smooth() {
        const auto n = trails.size();
        for (size_t i = 0; i < n; ++i) {
            trails[i] = trails[i] - rho + tau_min;
            trails[i] = std::max(trails[i], trail_min);
            trails[i] = std::min(trails[i], trail_max);
        }
    }

    void increase_smooth(uint32_t node) {
        assert(node < dimension_);
        trails[node] = trails[node] - rho + tau_max;
        trails[node] = std::max(trails[node], trail_min);
        trails[node] = std::min(trails[node], trail_max);
    }
};