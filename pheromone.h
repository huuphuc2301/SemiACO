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


struct MatrixPheromone {
    uint32_t dimension_ = 0;
    std::vector<double> trails; // For every edge (a,b),
                                 // where 0 <= a, b < dimension_
    double rho;

    double delta = 0;
    double trail_min, trail_max;
    double deposit_smooth;

    MatrixPheromone(uint32_t dimension, double initial_pheromone)
        : dimension_(dimension),
          trails(dimension * dimension, initial_pheromone) {
    }

    double get(uint32_t from, uint32_t to) const {
        assert((from < dimension_) && (to < dimension_));
        return trails[from * dimension_ + to];
    }

    void init_smooth(double delta_) {
        delta = delta_;
    }

    void evaporate(double evaporation_rate, double min_pheromone_value) {
        const auto n = trails.size();

        #pragma omp for schedule(static)
        for (size_t i = 0; i < n; ++i) {
            trails[i] = std::max(min_pheromone_value, trails[i] * evaporation_rate);
        }
    }

    void evaporate_smooth(double evaporation_rate) {
        const auto n = trails.size();

        #pragma omp for schedule(static)
        for (size_t i = 0; i < n; ++i) {
            trails[i] = trails[i] * evaporation_rate + delta;
        }
    }

    void increase(uint32_t from, uint32_t to, double deposit,
                  double max_pheromone_value) {

        assert((from < dimension_) && (to < dimension_));

        auto &value = trails[from * dimension_ + to];
        value = std::min(max_pheromone_value, value + deposit);
    }
};