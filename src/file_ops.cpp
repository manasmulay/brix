#include "file_ops.h"

#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define CHUNK_SIZE 1024*60
#define PORT 8081

using namespace std;

string get_file_name(string file_path) {
  string ret;
  int i = file_path.length();
  while(file_path[--i] != '/') {}
  return file_path.substr(++i);
}

int split_and_send_files(string file_path, string storage_folder_path) {
  int file_split_counter = 1;
  string file_name, output_file_name;
  file_name = get_file_name(file_path);
  ifstream file_stream;
  cout << file_name << endl;
  file_stream.open(file_path, ios::in | ios::binary);
  if(file_stream.is_open()) {
    char *buffer;
    ofstream output;
    buffer = (char *) malloc(sizeof(char) * CHUNK_SIZE);
	int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");
		return -1;
	}
    while(!file_stream.eof()) {
      // Generate the file name for the chunk
      output_file_name = generate_op_file_name(storage_folder_path, file_name, file_split_counter);

      // Write the contents
      // output.open(output_file_name.c_str(),ios::out | ios::trunc | ios::binary);

      //if(output.is_open()) {
        file_stream.read(buffer, CHUNK_SIZE);
        //output.write(buffer, file_stream.gcount());
        //output.close();
		cout << sizeof(buffer) << endl;
		cout << "Sending buffer size: " << file_stream.gcount() << endl;
    	send(sock, buffer, file_stream.gcount(), 0);
	    printf("Hello message sent\n");
		fsync(sock);
        read(sock, buffer, 1024);
		file_split_counter += 1;
      //}
    }
	buffer = "exit";
	send(sock, buffer, strlen(buffer),0);
    //delete(buffer);
    file_stream.close();
	close(sock);
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
  cout << "Merging files ...\n";
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

string generate_op_file_name(string storage_folder_path, string file_name, int file_split_counter) {
  string output_file_name;
  output_file_name.clear();
  output_file_name = storage_folder_path;
  output_file_name.append(file_name);
  output_file_name.append(".part.");
  output_file_name.append(to_string(file_split_counter));
  return output_file_name;
}
