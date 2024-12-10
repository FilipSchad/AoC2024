#pragma once

#include <filesystem>
#include <fstream>

namespace utils
{
std::filesystem::path abs_exe_directory();
void load_columns(std::vector<int>& col1, std::vector<int>& col2, const std::filesystem::path& file_path);
std::vector<std::vector<int>> load_rows(const std::filesystem::path& file_path);
}  // namespace utils
