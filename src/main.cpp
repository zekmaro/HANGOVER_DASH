#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <immintrin.h>
#include <iostream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <vector>

struct __attribute__((packed)) Node {
    int val;
    bool visited;

    Node(int val) : val(val), visited(false) {}
};

struct __attribute__((packed)) Pos {
    int col;
    int row;
};

__attribute__((always_inline)) static inline std::pair<std::string, int>
solve(std::vector<std::vector<Node>> &map, const Pos start_pos, const Pos end_pos) {
    int rows = map.size();
    int cols = map[0].size();

    std::priority_queue<std::tuple<int, int, int>, std::vector<std::tuple<int, int, int>>, std::greater<>> open_list;
    std::vector<std::vector<int>> min_cost(rows, std::vector<int>(cols, std::numeric_limits<int>::max()));
    std::vector<std::vector<Pos>> parent(rows, std::vector<Pos>(cols, {-1, -1}));

    const static std::vector<std::pair<int, int>> directions = {
        {-1, 0 },
        {1,  0 },
        {0,  -1},
        {0,  1 }
    };

    const static std::vector<char> direction_chars = {'U', 'D', 'L', 'R'};

    const int start_x = start_pos.row;
    const int start_y = start_pos.col;

    min_cost[start_x][start_y] = 0;

    open_list.push({0, start_x, start_y});

    while (!open_list.empty()) {
        auto [current_cost, x, y] = open_list.top();
        open_list.pop();

        if (map[x][y].visited) {
            continue;
        }

        map[x][y].visited = true;

        if (x == end_pos.row && y == end_pos.col) {
            break;
        }

        for (size_t d = 0; d < directions.size(); ++d) {
            int nx = x + directions[d].first;
            int ny = y + directions[d].second;

            if (nx < 0 || ny < 0 || nx >= rows || ny >= cols) {
                continue;
            }

            const Node &neighbor = map[nx][ny];
            if (neighbor.visited) {
                continue;
            }

            const int new_cost = current_cost + neighbor.val;

            if (new_cost < min_cost[nx][ny]) {
                min_cost[nx][ny] = new_cost;
                parent[nx][ny] = {x, y};
                open_list.push({new_cost, nx, ny});
            }
        }
    }

    std::string path;
    int total_cost = min_cost[end_pos.row][end_pos.col], x = end_pos.row, y = end_pos.col;

    while (!(x == start_pos.row && y == start_pos.col)) {
        auto [px, py] = parent[x][y];
        for (size_t d = 0; d < directions.size(); ++d) {
            if (px + directions[d].first == x && py + directions[d].second == y) {
                path += direction_chars[d];
                break;
            }
        }
        x = px;
        y = py;
    }
    std::reverse(path.begin(), path.end());
    return {path, total_cost};
}

int
main(int ac, char **av) {
    std::ifstream file(av[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    std::vector<std::string> mapVec;
    std::string line;
    while (std::getline(file, line)) {
        mapVec.push_back(line);
    }

    Pos start_pos, end_pos;

    static std::unordered_map<short, std::vector<short>> coefficients;

    coefficients['W'] = {5, 5, 0};
    coefficients['A'] = {5, 0, 5};
    coefficients['E'] = {0, 5, 5};

    std::vector<float> conversion = {4, 3, 2.5, 2, 1.5, 1};

    std::vector<std::vector<Node>> A, B, C;

    for (int row = 0; row < (int)mapVec.size(); ++row) {
        std::vector<Node> A0, B0, C0;

        for (int col = 0; col < (int)mapVec[row].size(); col += 2) {
            if (col + 1 >= (int)mapVec[row].size()) {
                std::cerr << "Invalid map format at row " << row << ", column " << col << std::endl;
                return 1;
            }

            const uint_fast16_t entry = (mapVec[row][col + 1] << 8) | mapVec[row][col];

            if ((entry & 0xFF) == 'M') {
                start_pos = Pos{col / 2, row};
                A0.push_back(Node(0));
                B0.push_back(Node(0));
                C0.push_back(Node(0));
            } else if ((entry & 0xFF) == 'G') {
                end_pos = Pos{col / 2, row};
                A0.push_back(Node(0));
                B0.push_back(Node(0));
                C0.push_back(Node(0));
            } else {
                A0.push_back(Node((entry >> 8) * conversion[coefficients[entry & 0xFF][0]]));
                B0.push_back(Node((entry >> 8) * conversion[coefficients[entry & 0xFF][1]]));
                C0.push_back(Node((entry >> 8) * conversion[coefficients[entry & 0xFF][2]]));
            }
        }

        A.push_back(A0);
        B.push_back(B0);
        C.push_back(C0);
    }

    std::vector<std::pair<std::string, int>> results;
    std::pair<std::string, int> bestSolution;

    results.push_back(solve(A, start_pos, end_pos));
    bestSolution = results[0];
    std::cout << coefficients['W'][0] << coefficients['A'][0] << coefficients['E'][0];
    std::cout << bestSolution.first << std::endl;

    results.push_back(solve(B, start_pos, end_pos));
    if (results[1].second < bestSolution.second) {
        std::cout << coefficients['W'][1] << coefficients['A'][1] << coefficients['E'][1];
        std::cout << bestSolution.first << std::endl;
    }

    results.push_back(solve(C, start_pos, end_pos));
    if (results[2].second < bestSolution.second) {
        std::cout << coefficients['W'][2] << coefficients['A'][2] << coefficients['E'][2];
        std::cout << bestSolution.first << std::endl;
    }

    return 0;
}
