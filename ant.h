#pragma once

#include "utils.h"


struct RouteIterator {
    const std::vector<uint32_t> &route_;
    size_t position_ = 0;

    uint32_t goto_succ() {
        position_ = (position_ + 1 < route_.size()) ? position_ + 1 : 0;
        return route_[position_];
    }

    uint32_t goto_pred() {
        position_ = position_ != 0 ? position_ - 1 : route_.size() - 1;
        return route_[position_];
    }
};


struct Solution {
    std::vector<uint32_t> route_;
    std::vector<uint32_t> node_indices_;

    Solution() = default;

    void add(uint32_t node, double cost) {
        route_.push_back(node);
        node_indices_[node] = route_.size() - 1;
    }

    void update(const Solution *other) {
        route_ = other->route_;
        update_node_indices();
    }

    void update_node_indices() {
        for (size_t i = 0; i < route_.size(); ++i) {
            node_indices_[route_[i]] = static_cast<uint32_t>(i);
        }
    }

    // We assume that route is undirected
    bool contains_edge(uint32_t edge_head, uint32_t edge_tail) const {
        return get_succ(edge_head) == edge_tail   // same edge
            || get_pred(edge_head) == edge_tail;  // reversed
    }

    uint32_t get_succ(uint32_t node) const {
        auto index = node_indices_[node];
        return route_[(index + 1u < route_.size()) ? index + 1u : 0u];
    }

    uint32_t get_pred(uint32_t node) const {
        auto index = node_indices_[node];
        return route_[(index > 0u) ? index - 1u : route_.size() - 1u];
    }
};


struct Ant : public Solution {
    std::vector<uint32_t> unvisited_;
    Bitmask  visited_bitmask_;
    uint32_t dimension_ = 0;
    uint32_t visited_count_ = 0;
    std::vector<int> visited_;
    double total_MI = 0;
    std::pair<double, double> statistic;
    double accuracy = 0;
    double f_score = 0;

    Ant() : Solution() {}

    void initialize(uint32_t dimension) {
        dimension_ = dimension;
        visited_count_ = 0;

        route_.resize(dimension);

        unvisited_.resize(dimension);
        std::iota(unvisited_.begin(), unvisited_.end(), 0);

        visited_bitmask_.resize(dimension);
        visited_bitmask_.clear();
    }

    void visit(uint32_t node) {
        assert(!is_visited(node));

        route_[visited_count_++] = node;
        visited_bitmask_.set_bit(node);
        visited_.push_back(node);
    }

    bool is_visited(uint32_t node) const {
        return visited_bitmask_.get_bit(node);
    }


    uint32_t get_current_node() const {
        return route_[visited_count_ - 1];
    }

    uint32_t get_unvisited_count() const {
        return dimension_ - visited_count_;
    }

    const std::vector<uint32_t> &get_unvisited_nodes() {
        // Filter out visited nodes from unvisited_ list that
        // now can be invalid
        //
        // This has linear complexity but should not be a problem
        // if this method is not called very often.
        size_t n = unvisited_.size();
        size_t j = 0;
        for (size_t i = 0; i < n; ++i) {
            auto node = unvisited_[i];
            if (!is_visited(node)) {
                unvisited_[j++] = node;
            }
        }
        assert(j == get_unvisited_count());
        unvisited_.resize(j);
        return unvisited_;
    }
};
