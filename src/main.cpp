#include "file_ops.h"
#include "hash_functions.h"
#include <string>
#include <iostream>


#define FILE_PATH "../files/file"
#define STORAGE_FOLDER "../files/storage/"
#define MERGED_FILE "../files/storage/file"

using namespace std;

int main(int argc, char const *argv[]) {
  string hash_before_split = get_file_hash(FILE_PATH);
  split_file(FILE_PATH, STORAGE_FOLDER);
  merge_files(STORAGE_FOLDER);
  string hash_after_split = get_file_hash(MERGED_FILE);
  if(!hash_before_split.compare(hash_after_split)) {
    cout << "File merged successfully\n";
    return 0;
  }
  cout << "Error in file merging\n";
  return 1;
}
