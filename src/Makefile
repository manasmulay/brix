objects = file_ops.o hash_functions.o
all: $(objects) main.o file_receive_client.o
	g++ $(objects) main.o -o main.out -lcrypto
	g++ file_ops.o file_receive_client.o -o server.out 
file_ops.o: file_ops.cpp file_ops.h
	g++ -c file_ops.cpp -std=c++17
hash_functions.o: hash_functions.cpp hash_functions.h
	g++ -c hash_functions.cpp
main.o: main.cpp
	g++ -c main.cpp
file_receive_client.o: file_receive_client.cpp
	g++ -c file_receive_client.cpp
clean:
	rm -rf *.out
	rm -rf *.o
