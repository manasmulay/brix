# Brix
A simple PoC on distributed file storage using file splitting.
## Usage
Create a directory called **files** in the same directory as **src**.</br>
The files folder should contain your file to be split and also another directory called **storage**, where the split files will be stored.
```bash
├── files
│   ├── file
│   └── storage
├── README.md
└── src
    ├── *
```
## Running the program
Run the following commands in the **src** directory
```bash
make all
./main.out
```
Clearing all binaries
```bash
make clean
```
