#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "common.h"

#include <algorithm>
#include <format>
#include <fstream>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

void load_columns(std::vector<int>& col1, std::vector<int>& col2, const std::filesystem::path& file_path)
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
	std::ranges::sort(col1);
	std::ranges::sort(col2);
	int result = 0;
	for (std::pair<int&, int&> pair : std::views::zip(col1, col2)) {
		result += std::abs(pair.first - pair.second);
	}
	return result;
}

int solve_part2(const std::vector<int>& col1, const std::vector<int>& col2)
{
	int result = 0;
	for (auto val : col1) {
		result += val * std::ranges::count(col2, val);
	}
	return result;
}

int solve_day1(int part, const std::filesystem::path& input_file)
{
	std::vector<int> col1;
	std::vector<int> col2;
	load_columns(col1, col2, input_file);
	if (part == 1) {
		return solve_part1(col1, col2);
	}

	if (part == 2) {
		return solve_part2(col1, col2);
	}
	throw std::logic_error(std::format("Unknown part {}", part));
}

TEST_SUITE("Day1")
{
	auto small_input_file = abs_exe_directory() / "input" / "day1.small.txt";
	auto full_input_file = abs_exe_directory() / "input" / "day1.full.txt";

	TEST_CASE("Part1")
	{
		CHECK(solve_day1(1, small_input_file) == 11);
		CHECK(solve_day1(1, full_input_file) == 2'815'556);
	}

	TEST_CASE("Part2")
	{
		CHECK(solve_day1(2, small_input_file) == 31);
		CHECK(solve_day1(2, full_input_file) == 23'927'637);
	}
}
