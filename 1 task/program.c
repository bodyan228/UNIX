#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

    int block_size = 4096;
    char input_file[20];
    char output_file[20];

    int rez = 0;
    int input_flag = 0;

    int d_input_file;

    while ((rez = getopt(argc, argv, "i::o::b::")) != -1){
        switch (rez)
        {
        case 'i':
            strcpy(input_file, optarg);
            input_flag = 1;
            break;
        case 'o':
            strcpy(output_file, optarg);
            break;
        case 'b':
            block_size = atoi(optarg);
        }
    }
    
    int d_output_file = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    char buffer[block_size];
    int bytes_read;

    if (input_flag == 1){
        d_input_file = open(input_file, O_RDONLY);
    }else{
        d_input_file = fileno(stdin);
    }
    
    while ((bytes_read = read(d_input_file, buffer, block_size)) > 0){
        int not_zero_block = 0;
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] != 0) {
                not_zero_block = 1; 
                break;
            }
        }

        if (not_zero_block == 0){
            lseek(d_output_file, block_size, SEEK_CUR);             
        } else {
            write(d_output_file, buffer, block_size);                         
        }
    }    
    close(d_input_file);
    close(d_output_file);

    return 0;
}
