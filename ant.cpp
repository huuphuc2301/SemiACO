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
#include <omp.h>

#include "ant.h"
#include "pheromone.h"
#include "progargs.h"
#include "utils.h"
#include "rand.h"
#include "json.hpp"
#include "heuristic.h"

#define MAX_N 1005

using namespace std;


//choose the next node by probability
uint32_t select_next_node(const MatrixPheromone &pheromone,
                          const HeuristicData &heuristic,
                          const uint32_t current_node,
                          const std::vector<uint32_t> unvisited,
                          const Bitmask &visited) {

    double product_prefix_sums[unvisited.size()];
    double products_sum = 0;
    double max_prod = 0;
    ///calculate probability of each node
    

    ///random a real number

    ///find the range to get the result node
}

template<class Impl>
class ACOModel {
protected:
    double p_best_;
    double rho_;
    double tau_min_;
    double deposit_smooth_;
    uint32_t cand_list_size_;
public:
    Limits trail_limits_;

    calc_trail_limits_fn_t calc_trail_limits_ = calc_trail_limits;


    ACOModel(const ProgramOptions &options)
        , p_best_(options.p_best_)
        , rho_(options.rho_)
        , cand_list_size_(options.cand_list_size_)
        , tau_min_(options.tau_min_)
    {}

    void init(double solution_cost) {
        update_trail_limits(solution_cost);
        static_cast<Impl*>(this)->init_impl();
    }

    void update_trail_limits(double solution_cost) {
        ///
    }

    void init_trail_limits_smooth() {
        ///
    }
    void evaporate_pheromone() {
        get_pheromone().evaporate(1 - rho_, trail_limits_.min_);
    }

    void evaporate_pheromone_smooth() {
        get_pheromone().evaporate_smooth(1 - rho_);
    }

    decltype(auto) get_pheromone() {
        return static_cast<Impl*>(this)->get_pheromone_impl();
    }

    // Increases amount of pheromone on trails corresponding edges of the
    // given solution (sol). Returns deposited amount. 
    double deposit_pheromone(const Ant &sol) {
        const double deposit = 1.0 / sol.cost_;
        auto prev_node = sol.route_.back();
        auto &pheromone = get_pheromone();
        for (auto node : sol.route_) {
            // The global update of the pheromone trails
            pheromone.increase(prev_node, node, deposit, trail_limits_.max_);
            prev_node = node;
        }
        return deposit;
    }

    void deposit_pheromone_smooth(const Ant &sol) {
        auto prev_node = sol.route_.back();
        auto &pheromone = get_pheromone();
        for (auto node : sol.route_) {
            pheromone.increase(prev_node, node, deposit_smooth_, trail_limits_.max_);
            prev_node = node;
        }
    }
};

void run_origin_algo(const ProgramOptions &opt, const HeuristicData heuristic) {
    const auto ants_count = opt.ants_count_;
    const auto iterations = opt.iterations_;

    HeuristicData heuristic;

    Ant best_ant;

    vector<Ant> ants(ants_count);
    Ant *iteration_best = nullptr;

    ///run multiple ants
    /*for (int ite = 1;) {
        for (int ant = 1;) {
            
            //while (visited_edge < n_edge) 

            //update local pheromone
            //update local heuristic 
        }
        
        //update global pheromone
        //update global heuristic
    }*/
}

int32_t main() {
    ///initialize heuristicData from input
}



