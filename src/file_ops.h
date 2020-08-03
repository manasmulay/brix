#include <string>
#include <vector>

#ifndef FILE_OPS_H
#define FILE_OPS_H

/*
 * Get filename from file_path
 */
std::string get_file_name(std::string file_path);

/*
 * Split the file specified by file_path into chunks
 */
int split_and_send_files(std::string file_path, std::string storage_folder_path);

/*
 * Custom comparator to sort file chunks list
 */
bool cmp(std::string l, std::string r);

/*
 * Get a sorted vector of strings containing list of chunfile chunks
 */
std::vector<std::string> get_split_files_list(std::string storage_folder_path);

/*
 * Merge the file chunks back into one file
 */
int merge_files(std::string storage_folder_path);

/*
 * Get the file name for the file part
 */
std::string generate_op_file_name(std::string storage_folder_path, std::string file_name, int file_split_counter);
#endif
