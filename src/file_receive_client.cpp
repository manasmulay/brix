#include "file_ops.h"

#include <iostream>
#include <fstream>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>

#define PORT 8081
#define IP "127.0.0.1"
#define CHUNK_SIZE 1024*60
#define STORAGE_FOLDER "../files/storage/"

using namespace std;

int main(){
	int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char *buffer;
	buffer = (char *) malloc(sizeof(char) * CHUNK_SIZE);	
	int split_counter = 1;
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
	
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) { 
        	perror("accept");
        	exit(EXIT_FAILURE);
    }
	
    while(true) {
		ofstream output;
    	valread = read( new_socket , buffer, CHUNK_SIZE);
		if(buffer[0] == 'e' &&
			buffer[1] == 'x' &&
			buffer[2] == 'i' &&
			buffer[3] == 't') {
			send(new_socket,"recv",strlen("recv"), 0);
			close(new_socket);
			break;
		}
		string storage_folder = STORAGE_FOLDER;
		string file_name = generate_op_file_name(storage_folder, "input_file", split_counter++);
		cout << "Read bytes: " << valread << endl;
		send(new_socket, "read", 4, 0);
		output.open(file_name.c_str(), ios::out | ios::binary);
		if(output.is_open()) {
			output.write(buffer, valread);
			output.flush();
			output.close();
		}
	}

	return 0;
}
