// C++ standard libs
#include <algorithm>
#include <exception>
#include <queue>
#include <stack>
#include <tuple>
#include <unordered_set>

// Outside libs
#include <boost/functional/hash.hpp>
#include <personal.hpp>

// Local libs
#include "include/knight.hpp"

namespace kc{
//==================================================================================
// Tools
//==================================================================================
size_t knight_steps(size_t board_size, int8_t opt_level/*=0*/) {
    size_t solution;
    switch(opt_level) {
        case 0: solution = knight_steps_bfs(board_size); break;
        case 1: solution = knight_steps_astar(board_size); break;
        case 2: solution = knight_steps_parallel(board_size); break;
    }
    return solution;
}

void print_pos(const Pos& pos) {
    std::cout << "Pos: (" << pos.first << ", " << pos.second << ")\n";
}

void print_all(const Pos& pos, size_t steps) {
    std::cout << "Pos: (" << pos.first << ", " << pos.second 
        << "); Steps: " << steps << std::endl;
}

//==================================================================================
// Breadth First
//==================================================================================
size_t knight_steps_bfs(size_t board_size) {
    // Initialize
    int n = board_size;
    std::queue<std::pair<Pos, size_t>> options;
    Pos goal = std::make_pair(n-1, n-1);
    Pos start = std::make_pair(0,0);
    options.push(std::make_pair(start,0));
    std::unordered_set<Pos, boost::hash<Pos>> visited;

    std::vector<Pos> movement = {std::make_pair(1,2), std::make_pair(2,1),
        std::make_pair(2,-1), std::make_pair(1,-2)};

    auto bounds_check = [&](int num) -> bool {
        return ((num >= 0) && (num < n));
    };

    while (!options.empty()) {
        // Remove from queue
        auto current = options.front();
        options.pop();
        Pos current_pos = current.first;
        size_t current_steps = current.second;
        //print_all(current_pos, current_steps);

        // Check if visited before
        if (visited.count(current_pos) > 0) {
            continue;
        } else {
            visited.insert(current_pos);
        }
        
        // Check if done
        if (current_pos == goal) {
            return current_steps;
        }

        // Otherwise add new options to queue
        for (auto& delta: movement) {
            auto forward_x = current_pos.first + delta.first;
            auto forward_y = current_pos.second + delta.second;
            auto back_x = current_pos.first - delta.first;
            auto back_y = current_pos.second - delta.second;
            if ((bounds_check(forward_x)) && (bounds_check(forward_y))) {
                Pos forward_move = std::make_pair(forward_x, forward_y);
                options.push(std::make_pair(forward_move, current_steps+1));
            }
            if ((bounds_check(back_x)) && (bounds_check(back_y))) {
                Pos back_move = std::make_pair(back_x, back_y);
                options.push(std::make_pair(back_move, current_steps+1));
            }
        }
    }
    return 0;
}

//==================================================================================
// A* Star
//==================================================================================
size_t knight_steps_astar(size_t board_size) {
    // Initialize
    // We want to prioritize a lower weight
    // (Pos, steps, weight)
    using Option = std::tuple<Pos, size_t, int>;
    int n = board_size;
    auto queue_sort = [] (const Option& a, const Option& b) {
        return std::get<2>(a) > std::get<2>(b);
    };

    std::priority_queue<
        Option, std::vector<Option>, decltype(queue_sort)> 
        options(queue_sort);
    Pos goal = std::make_pair(n-1, n-1);
    Pos start = std::make_pair(0,0);
    auto calc_weight = [&](const Pos& current_pos, size_t steps) -> size_t {
        auto future = std::min(std::abs((current_pos.first - goal.first)),std::abs((current_pos.second - goal.second)));
        auto past = steps;
        return future + past;
    };
    options.push(std::make_tuple(start, 0, calc_weight(start,0)));
    std::unordered_set<Pos, boost::hash<Pos>> visited;

    std::vector<Pos> movement = {std::make_pair(1,2), std::make_pair(2,1),
        std::make_pair(2,-1), std::make_pair(1,-2)};

    auto bounds_check = [&](int num) -> bool {
        return ((num >= 0) && (num < n));
    };

    while (!options.empty()) {
        // Remove from queue
        auto current = options.top();
        options.pop();
        Pos current_pos = std::get<0>(current);
        int current_steps = std::get<1>(current);
        //print_all(current_pos, current_steps);

        // Check if visited before
        if (visited.count(current_pos) > 0) {
            continue;
        } else {
            visited.insert(current_pos);
        }
        
        // Check if done
        if (current_pos == goal) {
            return current_steps;
        }

        // Otherwise add new options to queue
        for (auto& delta: movement) {
            auto forward_x = current_pos.first + delta.first;
            auto forward_y = current_pos.second + delta.second;
            auto back_x = current_pos.first - delta.first;
            auto back_y = current_pos.second - delta.second;
            if ((bounds_check(forward_x)) && (bounds_check(forward_y))) {
                Pos forward_move = std::make_pair(forward_x, forward_y);
                options.push(std::make_tuple(
                    forward_move, current_steps+1,calc_weight(forward_move, current_steps+1)
                ));
            }
            if ((bounds_check(back_x)) && (bounds_check(back_y))) {
                Pos back_move = std::make_pair(back_x, back_y);
                options.push(std::make_tuple(
                    back_move, current_steps+1, calc_weight(back_move, current_steps+1)
                ));
            }
        }
    }
    return 0;
}

//==================================================================================
// Async
//==================================================================================
size_t knight_steps_async(size_t board_size) {
    // Initialize
    // We want to prioritize a lower weight
    // (Pos, steps, weight)
    using Option = std::tuple<Pos, size_t, int>;
    int n = board_size;
    auto queue_sort = [] (const Option& a, const Option& b) {
        return std::get<2>(a) > std::get<2>(b);
    };

    std::priority_queue<
        Option, std::vector<Option>, decltype(queue_sort)> 
        options(queue_sort);
    Pos goal = std::make_pair(n-1, n-1);
    Pos start = std::make_pair(0,0);
    auto calc_weight = [&](const Pos& current_pos, size_t steps) -> size_t {
        auto future = std::min(std::abs((current_pos.first - goal.first)),std::abs((current_pos.second - goal.second)));
        auto past = steps;
        return future + past;
    };
    options.push(std::make_tuple(start, 0, calc_weight(start,0)));
    std::unordered_set<Pos, boost::hash<Pos>> visited;

    std::vector<Pos> movement = {std::make_pair(1,2), std::make_pair(2,1),
        std::make_pair(2,-1), std::make_pair(1,-2)};

    auto bounds_check = [&](int num) -> bool {
        return ((num >= 0) && (num < n));
    };

    while (!options.empty()) {
        // Remove from queue
        auto current = options.top();
        options.pop();
        Pos current_pos = std::get<0>(current);
        int current_steps = std::get<1>(current);
        //print_all(current_pos, current_steps);

        // Check if visited before
        if (visited.count(current_pos) > 0) {
            continue;
        } else {
            visited.insert(current_pos);
        }
        
        // Check if done
        if (current_pos == goal) {
            return current_steps;
        }

        // Otherwise add new options to queue
        for (auto& delta: movement) {
            auto forward_x = current_pos.first + delta.first;
            auto forward_y = current_pos.second + delta.second;
            auto back_x = current_pos.first - delta.first;
            auto back_y = current_pos.second - delta.second;
            if ((bounds_check(forward_x)) && (bounds_check(forward_y))) {
                Pos forward_move = std::make_pair(forward_x, forward_y);
                options.push(std::make_tuple(
                    forward_move, current_steps+1,calc_weight(forward_move, current_steps+1)
                ));
            }
            if ((bounds_check(back_x)) && (bounds_check(back_y))) {
                Pos back_move = std::make_pair(back_x, back_y);
                options.push(std::make_tuple(
                    back_move, current_steps+1, calc_weight(back_move, current_steps+1)
                ));
            }
        }
    }
    return 0;
}

//==================================================================================
// Parallel
//==================================================================================
size_t knight_steps_parallel(size_t board_size) {
    return 0;
}

}

