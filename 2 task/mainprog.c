#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int access_lock = 0;
int deny_lock = 0;

//Обработчик для kill -2
void handler(int signal){
    if (signal == SIGINT){
        printf("Close porgramm with pid %d\n", getpid());
        FILE *result = fopen("result.txt", "a");
        fprintf(result, "Proccess with PID [%d] comleted access. Lock/deny: (%d/%d)\n", getpid(), access_lock, deny_lock);
        fclose(result);
        exit(0);
    }
}

void main(int argc, char *argv[]){
    char filename_lock[24];
    strcpy(filename_lock, argv[1]);
    strcat(filename_lock,".lck");
    signal(SIGINT, handler);
    while (1){
        //Пытаемся создать и читать файл атомарно, если получается, то он наш.
        FILE *file_lock = fopen(filename_lock, "wx");
        if (file_lock == NULL){
            deny_lock++;
            continue;
        } else{
            pid_t pid = getpid();
            fprintf(file_lock, "%d", pid);
            fflush(file_lock);
            //Открываем файл myfile и спим 1 сек.
            FILE *source_file = fopen(argv[1], "r+");
            sleep(1);
            fclose(source_file);
            char pid_from_file[10];
            // Проверка, что файл еще существует, если нет, то завершаемся
            if (file_lock == NULL){
                FILE *result = fopen("result.txt", "a");
                fprintf(result, "Proccess with PID [%d] comleted with error(open file with PID). Lock/deny: (%d/%d)\n", pid, access_lock, deny_lock);
                fclose(file_lock);
                return;
            }
            FILE *file_read = fopen(filename_lock,"r+");
            // Читаем PID из файла
            fgets(pid_from_file, 10, file_read);
            int pid_number_from_file = atoi(pid_from_file);
            // Если PID'ы совпадают, то ок, если нет, то завершаем процесс
            if (pid_number_from_file == pid){
                access_lock++;
                fclose(file_read);
                fclose(file_lock);
                remove(filename_lock);
            }else{
                FILE *result = fopen("result.txt", "a");
                fprintf(result, "Proccess with PID [%d] comleted with error. Compare 2 PID: in file[%d]. Lock/deny: (%d/%d)\n", pid, pid_number_from_file, access_lock, deny_lock);
                fclose(file_lock);
                return;
            }
        }
    }
}