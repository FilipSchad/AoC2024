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

bool is_ordered(const std::vector<std::pair<int, int>>& order_rules, std::vector<int> update)
{
	bool ok = true;
	for (const auto& [a, b] : order_rules) {
		const auto aIt = std::ranges::find(update, a);
		if (aIt != update.end()) {
			const auto bIt = std::ranges::find(update, b);
			if (bIt != update.end()) {
				if (std::distance(aIt, bIt) < 0) {
					ok = false;
					break;
				}
			}
		}
	}
	return ok;
}

void swap_wrong(const std::vector<std::pair<int, int>>& order_rules, std::vector<int>& update)
{
	for (std::size_t i = 0; i < update.size() - 1; ++i) {
		auto goodOrderIt = std::ranges::find(order_rules, std::make_pair(update[i], update[i + 1]));
		if (goodOrderIt == order_rules.end()) {
			auto wrongOrderIt = std::ranges::find(order_rules, std::make_pair(update[i + 1], update[i]));
			if (wrongOrderIt != order_rules.end()) {
				std::swap(update[i], update[i + 1]);
			}
		}
	}
}

int solve_day5(int part, const std::filesystem::path& input_file)
{
	std::ifstream in_file(input_file, std::ios::in | std::ios::binary);
	bool read_pairs = true;
	std::vector<std::pair<int, int>> order_rules;
	std::vector<std::vector<int>> updates;
	for (std::string line; std::getline(in_file, line);) {
		if (line != "\r") {
			if (read_pairs) {
				const auto pos = line.find('|');
				order_rules.emplace_back(std::stoi(line.substr(0, pos)),
				                         std::stoi(line.substr(pos + 1, line.size() - pos - 2)));
			}
			else {
				auto pos = line.find(',');
				std::vector<int> nums;
				nums.emplace_back(std::stoi(line.substr(0, pos)));
				while (pos != std::string::npos) {
					auto next_pos = line.find(',', pos + 1);
					if (next_pos != std::string::npos) {
						nums.emplace_back(std::stoi(line.substr(pos + 1, next_pos)));
					}
					else {
						nums.emplace_back(std::stoi(line.substr(pos + 1, line.size() - pos - 2)));
					}
					pos = next_pos;
				}
				updates.emplace_back(std::move(nums));
			}
		}
		else {
			read_pairs = false;
		}
	}

	
	// Verify rules over updates
	int result = 0;
	std::size_t update_idx = 0;
	std::vector<std::size_t> wrong_updates;
	for (const auto& update : updates) {
		if (is_ordered(order_rules, update)) {
			result += update[update.size() / 2];
		}
		else {
			wrong_updates.emplace_back(update_idx);
		}
		++update_idx;
	}

	if (part == 1) {
		return result;
	}
	
	else if (part == 2) {
		result = 0;
		// Fix wrong updates
		for (const auto idx : wrong_updates) {
			auto update = updates[idx];
			while (!is_ordered(order_rules, update)) {
				swap_wrong(order_rules, update);
			}
			result += update[update.size() / 2];
		}
		return result;
	}
	throw std::logic_error(std::format("Unknown part {}", part));
}

TEST_SUITE("Day5")
{
	auto small_input_file = utils::abs_exe_directory() / "input" / "day5.small.txt";
	auto full_input_file = utils::abs_exe_directory() / "input" / "day5.full.txt";

	TEST_CASE("Part1")
	{
		CHECK(solve_day5(1, small_input_file) == 143);
		CHECK(solve_day5(1, full_input_file) == 4'637);
	}

	TEST_CASE("Part2")
	{
		CHECK(solve_day5(2, small_input_file) == 123);
		CHECK(solve_day5(2, full_input_file) == 6'370);
	}
}

enum class Direction
{
	up = 0,
	right,
	down,
	left
};


// Returns true if there is a way out, false otherwise
bool find_way_out(std::pair<int, int> cur_pos, std::vector<std::vector<char>> field, int& visited_fields)
{
	Direction cur_direction = Direction::up;
	std::vector<std::tuple<int, int, Direction>> visited{};
	bool loop_detected = false;
	while (true) {
		std::pair<int, int> next_pos;
		switch (cur_direction) {
		case Direction::up:
			next_pos = {cur_pos.first - 1, cur_pos.second};
			break;
		case Direction::right:
			next_pos = {cur_pos.first, cur_pos.second + 1};
			break;
		case Direction::down:
			next_pos = {cur_pos.first + 1, cur_pos.second};
			break;
		case Direction::left:
			next_pos = {cur_pos.first, cur_pos.second - 1};
			break;
		default:
			next_pos = {cur_pos.first, cur_pos.second};
			break;
		}

		// Is it the end?
		if (next_pos.first == -1 || next_pos.first == field.size() || next_pos.second == -1
		    || next_pos.second == field.size()) {
			// Next position is out of field. End.
			break;
		}

		// There is barier - change direction and stay on the same field
		if (field[next_pos.first][next_pos.second] == '#') {
			cur_direction = static_cast<Direction>((static_cast<int>(cur_direction) + 1) % 4);
			continue;
		}

		// Loop protection
		auto next_move = std::make_tuple(next_pos.first, next_pos.second, cur_direction);
		auto already_visited = std::ranges::find(visited, next_move);
		if (already_visited != visited.end()) {
			loop_detected = true;
			break;
		}
		visited.emplace_back(next_move);

		// Normal field
		field[next_pos.first][next_pos.second] = 'X';
		cur_pos = next_pos;
	}

	// Sum all X
	visited_fields = 0;
	for (const auto& row : field) {
		visited_fields += static_cast<int>(std::ranges::count(row, 'X'));
	}

	return !loop_detected;
}

int solve_day6(int part, const std::filesystem::path& input_file)
{
	auto field = utils::load_rows<char>(input_file);
	std::pair<int, int> cur_pos{0, 0};
	bool found = false;
	for (std::size_t row_idx = 0; row_idx < field.size(); ++row_idx) {
		for (std::size_t col_idx = 0; col_idx < field[cur_pos.first].size(); ++col_idx) {
			if (field[row_idx][col_idx] == '^') {
				cur_pos = {row_idx, col_idx};
				found = true;
				break;
			}
		}
		if (found) {
			break;
		}
	}

	if (part == 1) {
		int result = 0;
		if (find_way_out(cur_pos, field, result)) {
			return result;
		}
		throw std::logic_error(std::format("Cannot finnish the task"));
	}

	if (part == 2) {
		int result = 0;
		int visited_fields = 0;
		for (std::size_t row_idx = 0; row_idx < field.size(); ++row_idx) {
			for (std::size_t col_idx = 0; col_idx < field[cur_pos.first].size(); ++col_idx) {
				// Place an extra obstacle and try to find if there will be a loop
				if (field[row_idx][col_idx] == '.') {
					auto modified_field = field;
					modified_field[row_idx][col_idx] = '#';
					if (!find_way_out(cur_pos, modified_field, visited_fields)) {
						++result;
					}
				}
			}
		}
		return result;
	}
	throw std::logic_error(std::format("Unknown part {}", part));
}

TEST_SUITE("Day6")
{
	auto small_input_file = utils::abs_exe_directory() / "input" / "day6.small.txt";
	auto full_input_file = utils::abs_exe_directory() / "input" / "day6.full.txt";

	TEST_CASE("Part1")
	{
		CHECK(solve_day6(1, small_input_file) == 41);
		CHECK(solve_day6(1, full_input_file) == 4'967);
	}

	TEST_CASE("Part2")
	{
		CHECK(solve_day6(2, small_input_file) == 6);
		// Extremely inefective. Do not run unless you really need it
		//CHECK(solve_day6(2, full_input_file) == 1'789);
	}
}

enum class Operation {
	mul,
	add,
	join
};

void get_combinations(std::size_t sampleCount,
                      const std::vector<Operation>& options,
                      std::vector<Operation>& stack,
                      std::vector<std::vector<Operation>>& opers)
{
	if (stack.size() == sampleCount) {
		opers.push_back(stack);
	}
	else {
		// Add a new number, iterate over all possibilities
		stack.push_back(Operation::mul);
		for (auto op : options) {
			stack.back() = op;
			get_combinations(sampleCount, options, stack, opers);
		}
		stack.pop_back();
	}
}

std::int64_t solve_day7(int part, const std::filesystem::path& input_file)
{
	std::int64_t result = 0;
	std::ifstream in_file(input_file);
	for (std::string line; std::getline(in_file, line);) {
		const auto pos_dd = line.find(':');
		auto res = std::stoll(line.substr(0, pos_dd));
		std::stringstream num_str(line.substr(pos_dd + 2, line.size() - pos_dd));
		std::vector<int> nums;
		int n;
		while (num_str >> n) {
			nums.emplace_back(n);
		}
		std::vector<Operation> stack;
		std::vector<std::vector<Operation>> opers_list;
		if (part == 1) {
			get_combinations(nums.size() - 1, {Operation::mul, Operation::add}, stack, opers_list);
		}
		else {
			get_combinations(nums.size() - 1, {Operation::mul, Operation::add, Operation::join}, stack, opers_list);
		}
		bool res_found = false;
		for (const auto& opers : opers_list) {
			std::int64_t val = nums[0];
			for (std::size_t i = 1; i < nums.size(); ++i) {
				if (opers[i - 1] == Operation::mul) {
					val *= nums[i];
				}
				else if (opers[i - 1] == Operation::add) {
					val += nums[i];
				}
				else if (opers[i - 1] == Operation::join) {
					std::stringstream v;
					v << val << nums[i];
					val = std::stoll(v.str());
				}
			}
			if (val == res) {
				res_found = true;
				break;
			}
		}
		if (res_found) {
			result += res;
		}
	}
	return result;
}

TEST_SUITE("Day7")
{
	auto small_input_file = utils::abs_exe_directory() / "input" / "day7.small.txt";
	auto full_input_file = utils::abs_exe_directory() / "input" / "day7.full.txt";

	TEST_CASE("Part1")
	{
		CHECK(solve_day7(1, small_input_file) == 3'749);
		CHECK(solve_day7(1, full_input_file) == 14'711'933'466'277);
	}

	TEST_CASE("Part2")
	{
		CHECK(solve_day7(2, small_input_file) == 11'387);
		// Extremely inefective. Do not run unless you really need it
		//CHECK(solve_day7(2, full_input_file) == 286'580'387'663'654);
	}
}
