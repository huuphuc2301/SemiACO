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
#include "json.hpp"
#include "heuristic.h"
#include "rand.h"

#define MAX_N 1005

using namespace std;


//choose the next node by probability
uint32_t select_next_node(const MatrixPheromone &pheromone,
                          const HeuristicData &heuristic,
                          const uint32_t current_node,
                          const std::vector<uint32_t> unvisited,
                          Ant &ant) {
    double product_prefix_sums[unvisited.size()];

    double products_sum = 0;
    double max_prod = 0;
    const auto current_node = ant.get_current_node();

    ///calculate probability of each node and acculumate
    for (uint32_t i = 0; i < unvisited.size(); i++) {
        auto u = unvisited[i];
        double product = pheromone.get(current_node, u) * heuristic.get(current_node, u);
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
    double p_best;
    double rho;
    double tau_min;
    double deposit_smooth;
    double trail_min, trail_max;
    std::unique_ptr<MatrixPheromone> pheromone_ = nullptr;

    MinMaxACOModel(const ProgramOptions &options, uint32_t dimension_)
        : p_best(options.p_best_)
        , rho(options.rho_)
        , tau_min(options.tau_min_) 
        , dimension(dimension_) {
        init_trail_limits_smooth();
        pheromone_ = std::make_unique<MatrixPheromone>(dimension, trail_max);
    }
    
    void init_trail_limits_smooth() {
        trail_max = 1.0;
        trail_min = 1.0 / pow(dimension, 1.0 / 4);
        deposit_smooth = rho * (trail_max - trail_min);
        get_pheromone().init_smooth(rho * trail_min);
    }

    void update_trail_limits(double solution_cost) {
        ///
    }

    MatrixPheromone &get_pheromone() { return *pheromone_;}

    void evaporate_pheromone() {
        get_pheromone().evaporate(1 - rho, trail_min);
    }

    void evaporate_pheromone_smooth() {
        get_pheromone().evaporate_smooth(1 - rho);
    }

    // Increases amount of pheromone on trails corresponding edges of the
    // given solution (sol). Returns deposited amount. 
    double deposit_pheromone(const Ant &sol) {
        const double deposit = 1.0 / sol.cost_;
        auto prev_node = sol.route_.back();
        auto &pheromone = get_pheromone();
        for (auto node : sol.route_) {
            // The global update of the pheromone trails
            pheromone.increase(prev_node, node, deposit, trail_max);
            prev_node = node;
        }
        return deposit;
    }

    void deposit_pheromone_smooth(const Ant &sol) {
        auto prev_node = sol.route_.back();
        auto &pheromone = get_pheromone();
        for (auto node : sol.route_) {
            pheromone.increase(prev_node, node, deposit_smooth, trail_max);
            prev_node = node;
        }
    }
};

void run_origin_algo(const ProgramOptions &opt, const HeuristicData &heuristic) {
    const auto ants_count = opt.ants_count_;
    const auto iterations = opt.iterations_;
    const auto target = heuristic.target_;
    const auto n = heuristic.dimension_;
    Ant best_ant;

    vector<Ant> ants(ants_count);
    
    MinMaxACOModel model(opt, n);
    ///run multiple ants
    for (uint32_t iteration = 0 ; iteration < iterations ; ++iteration) {
        Ant iteration_best;
        for (uint32_t antId = 1; antId <= ants_count; antId++) {
            Ant ant;
            ant.initialize(n);

            uint32_t current_node;
            //select start node 

            ant.visit(current_node);
            //visit target number nodes
            for (uint32_t i = 1; i < target; i++) {
                auto next_node = select_next_node(
                    model.get_pheromone(), 
                    heuristic, 
                    current_node, 
                    ant.get_unvisited_nodes(), 
                    ant
                );
                ant.visit(next_node);
                //get cost for new node
                //todo
                double cost = 0;
                
                ant.add(next_node, cost);
                current_node = next_node;
            }
            //update ant solution
            //compare ant and iteration best
            if (ant.cost_ < iteration_best.cost_) {
                iteration_best = ant;
            }
        }
        //update global pheromone
        model.evaporate_pheromone();
        model.deposit_pheromone(iteration_best);
        //...

        //update global heuristic
        //todo


        //update best ant
        if (iteration_best.cost_ < best_ant.cost_) {
            best_ant = iteration_best;
        }
        //...
    }
}

int32_t main() {
    ///initialize heuristicData from input
}



