#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "common.h"

#include <algorithm>
#include <format>
#include <numeric>
//#include <print>
#include <ranges>
#include <vector>

int solve_day1_part1(std::vector<int>& col1, std::vector<int>& col2)
{
	std::ranges::sort(col1);
	std::ranges::sort(col2);
	int result = 0;
	for (std::pair<int&, int&> pair : std::views::zip(col1, col2)) {
		result += std::abs(pair.first - pair.second);
	}
	return result;
}

int solve_day1_part2(const std::vector<int>& col1, const std::vector<int>& col2)
{
	int result = 0;
	for (auto val : col1) {
		result += val * static_cast<int>(std::ranges::count(col2, val));
	}
	return result;
}

int solve_day1(int part, const std::filesystem::path& input_file)
{
	std::vector<int> col1;
	std::vector<int> col2;
	utils::load_columns(col1, col2, input_file);
	if (part == 1) {
		return solve_day1_part1(col1, col2);
	}

	if (part == 2) {
		return solve_day1_part2(col1, col2);
	}
	throw std::logic_error(std::format("Unknown part {}", part));
}

bool row_is_stable(const std::vector<int>& row)
{
	std::vector<int> adj_row(row.size());
	std::adjacent_difference(row.begin(), row.end(), adj_row.begin());
	const bool stable_decreasing = std::all_of(adj_row.begin()+1, adj_row.end(), [](auto val) { return val < 0 && val > -4;});
	const bool stable_increasing = std::all_of(adj_row.begin()+1, adj_row.end(), [](auto val) { return val > 0 && val < 4;});
	if (stable_increasing || stable_decreasing) {
		return true;
	}
	return false;
}

int solve_day2(int part, const std::filesystem::path& input_file)
{
	if (part != 1 && part != 2) {
		throw std::logic_error(std::format("Unknown part {}", part));
	}

	auto rows = utils::load_rows(input_file);
	const bool use_dampener = part == 2;
	int result = 0;

	for (const auto& row : rows) {
		if (row_is_stable(row)) {
			++result;
		}
		else if (use_dampener) {
			for (std::size_t idx = 0; idx < row.size(); ++idx) {
				auto row2 = row;
				row2.erase(row2.begin()+idx);
				if (row_is_stable(row2)) {
					++result;
					break;
				}
			}
		}
	}

	return result;
}

TEST_SUITE("Day1")
{
	auto small_input_file = utils::abs_exe_directory() / "input" / "day1.small.txt";
	auto full_input_file = utils::abs_exe_directory() / "input" / "day1.full.txt";

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

TEST_SUITE("Day2")
{
	auto small_input_file = utils::abs_exe_directory() / "input" / "day2.small.txt";
	auto full_input_file = utils::abs_exe_directory() / "input" / "day2.full.txt";

	TEST_CASE("Part1")
	{
		CHECK(solve_day2(1, small_input_file) == 2);
		CHECK(solve_day2(1, full_input_file) == 218);
	}

	TEST_CASE("Part2")
	{
		CHECK(solve_day2(2, small_input_file) == 4);
		CHECK(solve_day2(2, full_input_file) == 290);
	}
}
