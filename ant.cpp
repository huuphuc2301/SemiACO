#include <cassert>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <ostream>
#include <random>
#include <stdexcept>
#include <vector>
#include <memory>
#include <functional>
#include <filesystem>

#include "ant.h"
#include "pheromone.h"
#include "progargs.h"
#include "utils.h"
#include "rand.h"
#include "heuristic.h"
#include "rand.h"
#include "knn.h"

#define MAX_N 1005

using namespace std;


//choose the next node by probability
uint32_t select_next_node(const double q0,
                          const MatrixPheromone &pheromone,
                          const HeuristicData &heuristic,
                          const std::vector<uint32_t> unvisited) {
    double product_prefix_sums[unvisited.size()];

    double products_sum = 0;
    double max_prod = 0;

    ///calculate probability of each node and acculumate
    for (uint32_t i = 0; i < unvisited.size(); i++) {
        auto u = unvisited[i];
        double product = pheromone.get(u) * heuristic.get(u);
        product_prefix_sums[i] = product;
        if (i > 0) product_prefix_sums[i] += product_prefix_sums[i - 1];
    }

    ///random a real number
    random_t rnd;
    long long mod = 1e9;
    double r = rnd.next(mod) / (double) mod;
    r *= product_prefix_sums[unvisited.size() - 1];

    ///find the range to get the result node
    for (uint32_t i = 0; i < unvisited.size(); i++) {
        if (r < product_prefix_sums[i]) return unvisited[i];
    }
    return unvisited[0];
}

struct MinMaxACOModel {
    uint32_t dimension;

    MatrixPheromone pheromone_;

    MinMaxACOModel(const ProgramOptions &options, uint32_t dimension_)
        : dimension(dimension_) {
        init_trail_limits_smooth(options);
    }

    MatrixPheromone get_pheromone() {
        return pheromone_;
    }

    void init_trail_limits_smooth(const ProgramOptions &options) {
        pheromone_.init_smooth(dimension, options);
    }

    void evaporate_pheromone_smooth() {
        pheromone_.evaporate_smooth();
    }

    void deposit_pheromone_smooth(const Ant &sol) {
        for (int node : sol.visited_) {
            pheromone_.increase_smooth(node);
        }
    }
};

void run_origin_algo(const ProgramOptions &opt, const HeuristicData &heuristic) {
    const auto ants_count = opt.ants_count_;
    const auto iterations = opt.iterations_;
    const auto target = opt.target_;
    const auto n = heuristic.dimension_;
    Ant best_ant;
    const int numFeature = heuristic.numFeature;

    vector<Ant> ants(ants_count);

    KNN knn;
    knn.init(heuristic.labeledSamples, heuristic.labels, 0.8);

    MinMaxACOModel model(opt, n);
    ///run multiple ants
    for (uint32_t iteration = 0 ; iteration < iterations ; ++iteration) {
        for (uint32_t antId = 1; antId <= ants_count; antId++) {
            Ant ant;
            ant.initialize(n);

            uint32_t current_node = select_next_node(opt.q0, model.get_pheromone(), heuristic, ant.get_unvisited_nodes());
            //select start node 

            ant.visit(current_node);
            //visit target number nodes
            for (uint32_t i = 1; i < target; i++) {
                auto next_node = select_next_node(
                        opt.q0,
                    model.get_pheromone(),
                    heuristic,
                    ant.get_unvisited_nodes()
                );
                ant.visit(next_node);

            }
            ant.accuracyRate = knn.calculateAccuracy(ant.visited_);

            if (ant.accuracyRate > best_ant.accuracyRate) {
                best_ant = ant;
            }
        }
        model.evaporate_pheromone_smooth();
        model.deposit_pheromone_smooth(best_ant);


    }
    int x = 1;
}

int32_t main() {
    HeuristicData heuristicData("./test/musk2_after_split.csv");

    ProgramOptions programOptions;
    run_origin_algo(programOptions, heuristicData);
}



