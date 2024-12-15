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

bool row_is_stable(const std::vector<int>& row)
{
	std::vector<int> adj_row(row.size());
	std::adjacent_difference(row.begin(), row.end(), adj_row.begin());
	const bool stable_decreasing =
	    std::all_of(adj_row.begin() + 1, adj_row.end(), [](auto val) { return val < 0 && val > -4; });
	const bool stable_increasing =
	    std::all_of(adj_row.begin() + 1, adj_row.end(), [](auto val) { return val > 0 && val < 4; });
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

	auto rows = utils::load_rows<int>(input_file);
	const bool use_dampener = part == 2;
	int result = 0;

	for (const auto& row : rows) {
		if (row_is_stable(row)) {
			++result;
		}
		else if (use_dampener) {
			for (std::size_t idx = 0; idx < row.size(); ++idx) {
				auto row2 = row;
				row2.erase(row2.begin() + idx);
				if (row_is_stable(row2)) {
					++result;
					break;
				}
			}
		}
	}

	return result;
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


int solve_day3(int part, const std::filesystem::path& input_file)
{
	std::ifstream in_file(input_file, std::ios::in | std::ios::binary);
	const auto sz = std::filesystem::file_size(input_file);
	std::string line(sz, '\0');
	in_file.read(line.data(), sz);

	auto pos = line.find("mul(", 0);
	auto disabling_pos = line.find("don't()", 0);

	// Do not use disabled parts in Part1
	if (part == 1) {
		disabling_pos = std::string::npos;
	}
	int result = 0;
	while (pos != std::string::npos) {
		pos += 4;
		if (disabling_pos == std::string::npos || pos < disabling_pos) {
			std::string num1_str;
			std::string num2_str;
			while (pos < line.size() && std::isdigit(line[pos])) {
				num1_str += line[pos];
				++pos;
			}
			if (line[pos] == ',') {
				++pos;
				while (pos < line.size() && std::isdigit(line[pos])) {
					num2_str += line[pos];
					++pos;
				}
			}
			if (!num1_str.empty() && !num2_str.empty() && line[pos] == ')') {
				++pos;
				result += std::stoi(num1_str) * std::stoi(num2_str);
			}
		}
		else {
			pos = line.find("do()", pos);
			if (pos != std::string::npos) {
				pos += 4;
				disabling_pos = line.find("don't()", pos);
			}
		}
		
		if (pos != std::string::npos) {
			pos = line.find("mul(", pos);
		}
	}
	return result;
}

TEST_SUITE("Day3")
{
	auto small_input_file = utils::abs_exe_directory() / "input" / "day3.small.txt";
	auto full_input_file = utils::abs_exe_directory() / "input" / "day3.full.txt";

	TEST_CASE("Part1")
	{
		CHECK(solve_day3(1, small_input_file) == 161);
		CHECK(solve_day3(1, full_input_file) == 170'807'108);
	}

	TEST_CASE("Part2")
	{
		CHECK(solve_day3(2, small_input_file) == 48);
		CHECK(solve_day3(2, full_input_file) == 74'838'033);
	}
}

int solve_day4(int part, const std::filesystem::path& input_file)
{
	auto rows = utils::load_rows<char>(input_file);
	if (part == 1) {
		int xmas = 0;
		std::size_t row_idx = 0;
		for (const auto& row : rows) {
			std::size_t col_idx = 0;
			for (const auto c : row) {
				if (c == 'X') {
					// Try to find word XMAS in all 8 directions

					// Left to right
					if (col_idx + 3 < row.size()) {
						std::string x(row.begin() + col_idx, row.begin() + col_idx + 4);
						if (x == "XMAS") {
							++xmas;
						}
					}
					// Right to left
					if (col_idx > 2) {
						std::string x(row.begin() + col_idx - 3, row.begin() + col_idx + 1);
						std::reverse(x.begin(), x.end());
						if (x == "XMAS") {
							++xmas;
						}
					}
					// From botom up
					if (row_idx > 2) {
						std::array<char, 5> t{};
						t[0] = c;
						t[1] = rows[row_idx - 1][col_idx];
						t[2] = rows[row_idx - 2][col_idx];
						t[3] = rows[row_idx - 3][col_idx];
						t[4] = '\0';
						std::string x(t.data());
						if (x == "XMAS") {
							++xmas;
						}
					}
					// Top down
					if (row_idx + 3 < rows.size()) {
						std::array<char, 5> t{};
						t[0] = c;
						t[1] = rows[row_idx + 1][col_idx];
						t[2] = rows[row_idx + 2][col_idx];
						t[3] = rows[row_idx + 3][col_idx];
						t[4] = '\0';
						std::string x(t.data());
						if (x == "XMAS") {
							++xmas;
						}
					}

					// Diagonal up and right
					if (col_idx + 3 < row.size() && row_idx > 2) {
						std::array<char, 5> t{};
						t[0] = c;
						t[1] = rows[row_idx - 1][col_idx + 1];
						t[2] = rows[row_idx - 2][col_idx + 2];
						t[3] = rows[row_idx - 3][col_idx + 3];
						t[4] = '\0';
						std::string x(t.data());
						if (x == "XMAS") {
							++xmas;
						}
					}

					// Diagonal down and right
					if (col_idx + 3 < row.size() && row_idx + 3 < rows.size()) {
						std::array<char, 5> t{};
						t[0] = c;
						t[1] = rows[row_idx + 1][col_idx + 1];
						t[2] = rows[row_idx + 2][col_idx + 2];
						t[3] = rows[row_idx + 3][col_idx + 3];
						t[4] = '\0';
						std::string x(t.data());
						if (x == "XMAS") {
							++xmas;
						}
					}

					// Diagonal down and left
					if (col_idx > 2 && row_idx + 3 < rows.size()) {
						std::array<char, 5> t{};
						t[0] = c;
						t[1] = rows[row_idx + 1][col_idx - 1];
						t[2] = rows[row_idx + 2][col_idx - 2];
						t[3] = rows[row_idx + 3][col_idx - 3];
						t[4] = '\0';
						std::string x(t.data());
						if (x == "XMAS") {
							++xmas;
						}
					}

					// Diagonal up and left
					if (col_idx > 2 && row_idx > 2) {
						std::array<char, 5> t{};
						t[0] = c;
						t[1] = rows[row_idx - 1][col_idx - 1];
						t[2] = rows[row_idx - 2][col_idx - 2];
						t[3] = rows[row_idx - 3][col_idx - 3];
						t[4] = '\0';
						std::string x(t.data());
						if (x == "XMAS") {
							++xmas;
						}
					}
				}
				++col_idx;
			}
			++row_idx;
		}
		return xmas;
	}
	if (part == 2) {
		int xmas = 0;
		for (std::size_t row_idx = 1; row_idx < rows.size()-1; ++row_idx) {
			const auto& row = rows[row_idx];
			for (std::size_t col_idx = 1; col_idx < row.size() - 1; ++col_idx) {
				if (row[col_idx] == 'A') {
					if (((rows[row_idx - 1][col_idx - 1] == 'M' && rows[row_idx + 1][col_idx + 1] == 'S') || (rows[row_idx - 1][col_idx - 1] == 'S' && rows[row_idx + 1][col_idx + 1] == 'M'))
					    &&
						((rows[row_idx - 1][col_idx + 1] == 'M' && rows[row_idx + 1][col_idx - 1] == 'S') || (rows[row_idx - 1][col_idx + 1] == 'S' && rows[row_idx + 1][col_idx - 1] == 'M')))
					{
						++xmas;
					}
				}
			}
		}
		return xmas;
	}
	throw std::logic_error(std::format("Unknown part {}", part));
}


TEST_SUITE("Day4")
{
	auto small_input_file = utils::abs_exe_directory() / "input" / "day4.small.txt";
	auto full_input_file = utils::abs_exe_directory() / "input" / "day4.full.txt";

	TEST_CASE("Part1")
	{
		CHECK(solve_day4(1, small_input_file) == 18);
		CHECK(solve_day4(1, full_input_file) == 2'593);
	}

	TEST_CASE("Part2")
	{
		CHECK(solve_day4(2, small_input_file) == 9);
		CHECK(solve_day4(2, full_input_file) == 1'950);
	}
}
