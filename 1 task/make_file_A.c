#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


int main() {
    int rm_file_a = system("rm ./A_FILE 2>>/dev/null");
    int create_file = system("fallocate -l 4194304 ./A_FILE");
    int a_file = open("A_FILE", O_WRONLY);
    lseek(a_file, 0, SEEK_SET);
    write(a_file, "1", 1);
    lseek(a_file, 10000, SEEK_SET);
    write(a_file, "1", 1);
    lseek(a_file, 0, SEEK_END);
    write(a_file, "1", 1);
    return 0;
}
