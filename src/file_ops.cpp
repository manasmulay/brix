#include "file_ops.h"
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <filesystem>
#include <vector>
#include <algorithm>

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
    return 1;
  }
  return 0;
}

// Custom comparator to compare file paths and
// sort them based on id of file part
bool cmp(string l, string r) {
  string a = l.substr(l.find("part."));
  string b = r.substr(r.find("part."));
  int ax = stoi(a.substr(a.find(".")+1));
  int bx = stoi(b.substr(b.find(".")+1));
  return ax < bx;
}

vector<string> get_split_files_list(string storage_folder_path) {
  int file_counter = 0;
  vector<string> file_list;
  for (const auto & entry : filesystem::directory_iterator(storage_folder_path)) {
        file_list.push_back(entry.path());
        file_counter++;
  }

  // Get sorted list of all file parts and store in file_list
  sort(file_list.begin(), file_list.end(), cmp);

  return file_list;
}

int merge_files(string storage_folder_path) {
  vector<string> file_list = get_split_files_list(storage_folder_path);
  ofstream output;
  ifstream ifs;
  output.open((storage_folder_path + "file").c_str(),ios::out | ios::trunc | ios::binary);
  for (string s : file_list) {
    ifs.open(s, ios::in | ios::binary);
    char *buffer;
    buffer = (char *) malloc(sizeof(char) * CHUNK_SIZE);
    if(ifs.is_open()) {
      while(!ifs.eof()) {
        ifs.read(buffer, CHUNK_SIZE);
        output.write(buffer, ifs.gcount());
      }
    }
    ifs.close();
  }
  output.close();
  return 0;
}
