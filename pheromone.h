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
    std::vector<double> trails_; // For every edge (a,b),
                                 // where 0 <= a, b < dimension_
    bool is_symmetric_ = true;

    double delta = 0;

    MatrixPheromone(uint32_t dimension, double initial_pheromone, bool is_symmetric)
        : dimension_(dimension),
          trails_(dimension * dimension, initial_pheromone),
          is_symmetric_(is_symmetric) {
            ///initialize config of pheromone
    }

    [[nodiscard]] double get(uint32_t from, uint32_t to) const {
        assert((from < dimension_) && (to < dimension_));
        return trails_[from * dimension_ + to];
    }

    void init_smooth(double delta_) {
        delta = delta_;
    }

    void evaporate(double evaporation_rate, double min_pheromone_value) {
        const auto n = trails_.size();

        #pragma omp for schedule(static)
        for (size_t i = 0; i < n; ++i) {
            trails_[i] = std::max(min_pheromone_value, trails_[i] * evaporation_rate);
        }
    }

    void evaporate_smooth(double evaporation_rate) {
        const auto n = trails_.size();

        #pragma omp for schedule(static)
        for (size_t i = 0; i < n; ++i) {
            trails_[i] = trails_[i] * evaporation_rate + delta;
        }
    }

    void increase(uint32_t from, uint32_t to, double deposit,
                  double max_pheromone_value) {

        assert((from < dimension_) && (to < dimension_));

        auto &value = trails_[from * dimension_ + to];
        value = std::min(max_pheromone_value, value + deposit);

        if (is_symmetric_) {
            trails_[to * dimension_ + from] = value;
        }
    }
};