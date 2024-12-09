#include <algorithm>
#include <cassert>
#include <fstream>
#include <ranges>
#include <print>
#include <sstream>
#include <string>
#include <vector>

void load_columns(std::vector<int>& col1, std::vector<int>& col2, std::string file_path)
{
    std::ifstream in_file(file_path);
    for (std::string line; std::getline(in_file, line);) {
        std::istringstream istream(line);
        int first_col;
        int second_col;
        istream >> first_col >> second_col;
        col1.emplace_back(first_col);
        col2.emplace_back(second_col);
    }
}

int solve_part1(std::vector<int>& col1, std::vector<int>& col2)
{
    std::sort(col1.begin(), col1.end());
    std::sort(col2.begin(), col2.end());
    int result = 0;
    for (std::pair<int&, int&> pair : std::views::zip(col1, col2)) {
        result += std::abs(pair.first - pair.second);
    }
    return result;
}

int solve_part2(std::vector<int>& col1, std::vector<int>& col2)
{
    int result = 0;
    for (auto val : col1) {
        result += val * std::count(col2.begin(), col2.end(), val);
    }
    return result;
}

int main(int argc, const char** argv)
{
    if (argc != 3) {
        std::println(stderr, "Input specify part and input_file: day1 1|2 file.txt");
        return 1;
    }

    int part = std::stoi(argv[1]);
    if (part == 1 || part == 2) {
        std::vector<int> col1;
        std::vector<int> col2;
        load_columns(col1, col2, argv[2]);
        assert(col1.size() == col2.size());

        int result = 0;
        if (part == 1) {
            result = solve_part1(col1, col2);
        }
        if (part == 2) {
            result = solve_part2(col1, col2);
        }
        std::println("Result is {}", result);
    }
    else {
        std::println(stderr, "Input specify part and input_file: day1 1|2 file.txt");
        return 1;
    }
    return 0;
}
