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
#include "heuristic.h"
#include "knn.h"

#define MAX_N 1005

using namespace std;

Timer timer;


//choose the next node by probability
double totalCos(const HeuristicData &heuristic, vector<int> visited, int newNode) {
    double sum = 0;
    for (int i : visited) {
        sum += heuristic.cos[i][newNode];
    }
    return sum;
}

uint32_t select_next_node(const double q0,
                          const MatrixPheromone &pheromone,
                          const HeuristicData &heuristic,
                          const std::vector<uint32_t> unvisited,
                          const std::vector<int> visited) {
    double product_prefix_sums[unvisited.size()];

    double products_sum = 0;
    double max_prod = 0;

    double q = rnd.next();
    if (q <= q0) {
        //get max product node
        double maxx = 0;
        int res = 0;
        for (int u : unvisited) {
            double product = pheromone.get(u) * heuristic.getMutualInformation(u);
            if (visited.size() > 0) {
                product = product / totalCos(heuristic, visited, u);
            }
            if (product > maxx) {
                maxx = product;
                res = u;
            }
        }
        return res;
    }

    ///calculate probability of each node and acculumate
    for (uint32_t i = 0; i < unvisited.size(); i++) {
        auto u = unvisited[i];
        double product = pheromone.get(u) * heuristic.getMutualInformation(u) / totalCos(heuristic, visited, u);
        if (visited.size() > 0) {
            product = product / totalCos(heuristic, visited, u);
        }
        product_prefix_sums[i] = product;
        if (i > 0) product_prefix_sums[i] += product_prefix_sums[i - 1];
    }

    ///random a real number

    long long mod = 1e9;
    double r = rnd.next(mod) / (double) mod;
    r *= product_prefix_sums[unvisited.size() - 1];


    ///find the range to getMutualInformation the result node
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

    void evaporate_pheromone_smooth(vector<uint32_t> unvisited_nodes) {
        for (int node : unvisited_nodes) {
            pheromone_.evaporate_smooth(node);
        }
    }

    void deposit_pheromone_smooth(vector<int> nodes) {
        for (int node : nodes) {
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

    KNN knn;
    knn.init(heuristic.labeledSamples, heuristic.labels, 0.6);


    MinMaxACOModel model(opt, n);
    ///run multiple ants
    for (uint32_t iteration = 0 ; iteration < iterations ; ++iteration) {
        Ant iteration_best_ant;
        vector<Ant> ants;
        vector<pair<double, int> > MI_order;
        for (uint32_t antId = 1; antId <= ants_count; antId++) {
            Ant ant;
            ant.initialize(n);

            uint32_t current_node = select_next_node(opt.q0, model.get_pheromone(), heuristic, ant.get_unvisited_nodes(), ant.visited_);
            //select start node

            ant.visit(current_node);
            ant.total_MI += heuristic.getMutualInformation(current_node);
            //visit target number nodes
            for (uint32_t i = 1; i < target; i++) {
                auto next_node = select_next_node(
                        opt.q0,
                    model.get_pheromone(),
                    heuristic,
                    ant.get_unvisited_nodes(),
                    ant.visited_
                );
                ant.visit(next_node);
                ant.total_MI += heuristic.getMutualInformation(next_node);
            }
            ant.get_unvisited_nodes();
            ants.push_back(ant);
            MI_order.push_back(make_pair(ant.total_MI, antId - 1));
        }

//        choose iteration_best_ant
        sort(MI_order.begin(), MI_order.end());
        for (int i = ants_count - 1; i >= ants_count - 5; i--) {
            Ant ant = ants[MI_order[i].second];
            pair<double, double> statistic = knn.calculateStatistic(ant.visited_);
            ant.statistic = statistic;
            if (ant.statistic > iteration_best_ant.statistic) {
                iteration_best_ant = ant;
            }
        }

        model.evaporate_pheromone_smooth(iteration_best_ant.get_unvisited_nodes());
        model.deposit_pheromone_smooth(iteration_best_ant.visited_);

        if (iteration_best_ant.statistic > best_ant.statistic){
            best_ant = iteration_best_ant;
        }

        cout << "iteration " << iteration << ": " << '\n';
        cout << best_ant.statistic.first << ' ' << iteration_best_ant.statistic.first << ' ' << timer.get_elapsed_seconds() << '\n';
        for (double i : model.pheromone_.trails) cout << i << ' ';
        cout << '\n';
        cout << '\n';
    }
    cout << best_ant.accuracy;
    FILE *file = freopen("result.txt", "w", stdout);
    for (int i : best_ant.visited_) {
        cout << i << ", ";
    }
    fclose(file);
    cout << 1;
}

int32_t main() {
//    FILE *file = freopen("result.txt", "w", stdout);
//    for (int i = 0; i < 166; i++) {
//        cout << i << ", ";
//    }
    HeuristicData heuristicData("./test/musk2_train.csv");
    ProgramOptions programOptions;
    programOptions.tau_min_ = programOptions.tau_max_ / (2.0 * heuristicData.numFeature);
    rnd.setSeed(timer.get_elapsed_nanoseconds());
    run_origin_algo(programOptions, heuristicData);
}



