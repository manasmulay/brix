#include "file_ops.h"
#include "hash_functions.h"

#define FILE_PATH "../files/file"
#define STORAGE_FOLDER "../files/storage/"
using namespace std;

int main(int argc, char const *argv[]) {
  split_file(FILE_PATH, STORAGE_FOLDER);
  return 0;
}
