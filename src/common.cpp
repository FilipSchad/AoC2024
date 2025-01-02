#include "common.h"

#include <sstream>
#include <string>

#if defined(_MSC_VER)
#include <windows.h>
#else
#include <unistd.h>
#endif

std::filesystem::path utils::abs_exe_directory()
{
    #if defined(_MSC_VER)
        wchar_t path[FILENAME_MAX] = { 0 };
        GetModuleFileNameW(nullptr, path, FILENAME_MAX);
        return std::filesystem::path(path).parent_path().string();
    #else
        char path[FILENAME_MAX];
        ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
        return std::filesystem::path(std::string(path, (count > 0) ? count: 0)).parent_path().string();
    #endif
}

void utils::load_columns(std::vector<int>& col1, std::vector<int>& col2, const std::filesystem::path& file_path)
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
