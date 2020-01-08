#include "file_ops.h"
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>

#define CHUNK_SIZE 50

using namespace std;

string get_file_name(string file_path) {
  string ret;
  int i = file_path.length();
  while(file_path[--i] != '/') {}
  return file_path.substr(++i);
}

int split_file(string file_path, string storage_folder_path) {
  int file_split_counter = 1;
  string file_name, output_file_name;
  file_name = get_file_name(file_path);
  ifstream file_stream;
  file_stream.open(file_path, ios::in | ios::binary);
  if(file_stream.is_open()) {
    char *buffer;
    ofstream output;
    buffer = (char *) malloc(sizeof(char) * CHUNK_SIZE);
    while(!file_stream.eof()) {
      // Generate the file name for the chunk
      output_file_name.clear();
      output_file_name = storage_folder_path;
      output_file_name.append(file_name);
      output_file_name.append(".part.");
      output_file_name.append(to_string(file_split_counter));

      // Write the contents
      output.open(output_file_name.c_str(),ios::out | ios::trunc | ios::binary);

      if(output.is_open()) {
        file_stream.read(buffer, CHUNK_SIZE);
        output.write(buffer, file_stream.gcount());
        output.close();
        file_split_counter += 1;
      }
    }

    delete(buffer);
    file_stream.close();
  }
  else {
    cout << "Error opening file\n";
  }
  return 0;
}
