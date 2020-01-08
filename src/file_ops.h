#include <string>
#include <vector>

#ifndef FILE_OPS_H
#define FILE_OPS_H

std::string get_file_name(std::string file_path);
int split_file(std::string file_path, std::string storage_folder_path);
bool cmp(std::string l, std::string r);
std::vector<std::string> get_split_files_list(std::string storage_folder_path);
int merge_files(std::string storage_folder_path);
#endif
