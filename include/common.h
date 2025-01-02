#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

namespace utils
{
std::filesystem::path abs_exe_directory();
void load_columns(std::vector<int>& col1, std::vector<int>& col2, const std::filesystem::path& file_path);

template <typename T>
std::vector<std::vector<T>> load_rows(const std::filesystem::path& file_path)
{
	std::vector<std::vector<T>> ret;
	std::ifstream in_file(file_path);
	for (std::string line; std::getline(in_file, line);) {
		std::vector<T> row;
		std::istringstream istream(line);
		T col;
		while (istream >> col) {
			row.emplace_back(col);
		}
		ret.emplace_back(row);
	}
	return ret;
}

}  // namespace utils
