/*   smol is a small C compiler
 *   Copyright (C) 2022  D. Nigh Herndon
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "smol.h"

int main(int argc, char** argv){
    // If we get too many arguments, exit the program
    if (argc != 2){
        printf("Incorrect number of arguments\n");
        exit(1);
    }
    // Create an input file
    FILE* input_file;

    // The input from the command line is the file path
    char* input_path = argv[1];

    // Attempt to open the file for reading "r"
    input_file = fopen(input_path,"r");

    // If opening the file failed, we say that it could not open
    // and then exit the program
    if(!input_file){
        printf("Cannot open %s\n", input_path);
        exit(1);
    }

    // Move the file pointer to the end of the file
    fseek(input_file, 0, SEEK_END);
    
    // Determine the location of the file pointer
    // This tells us how many bytes the file is
    long int file_size = ftell(input_file);

    // Move the file pointer back to the beginning;
    fseek(input_file, 0, SEEK_SET);

    // Allocate a buffer that is the size of the file
    char* buffer = calloc(file_size, sizeof(char));
    if (buffer == NULL){
        printf("Memory not allocated\n");
        exit(1);
    }
    
    // Create a pointer to the beginning of the buffer
    char* inputStream = buffer;

    // Label to determine how many bytes have been read from the file
    size_t bytes_read = 0;

    // We now loop through the file and read the bites into the buffer
    for(;;){
        // Assigns the number of bytes read to a temp
        // The fread function reads data from the file and places it in
        // the buffer. We tell it that each element is the size of a char,
        // that we want to read the whole file, and we give it the pointer
        // to the file
        size_t tmp = fread(buffer, sizeof(char), file_size, input_file);

        // Sums total bytes read
        bytes_read += tmp; 

        // If it read no bytes, indicates end of file. Break the loop
        if (tmp == 0) break;    
    }

    // Sanity check to ensure that the bytes read equal the file size
    if (bytes_read != file_size){
        printf("Memory corrupted\n");
        exit(1);
    }
    // Close the input file. Free the memory
    fclose(input_file);

    
    TOKEN* token = tokenizer(inputStream, file_size);

    // Call Parser
    NODE* node = parse(token);
    
    // Call Code generator
    code_generator(node);

    return 0;
}
