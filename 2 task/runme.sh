#!/bin/bash

rm ./myfile.lck 2>>/dev/null
rm ./result.txt 2>>/dev/null
rm ./myfile 2>>/dev/null
rm ./nohup.out 2>>/dev/null

# Лист пидов, для килла
list_pid=();
touch myfile

# Цикл для запуска 10 задач в фоне
for i in {1..10}
do
    # Запускаем с nohup, чтобы процессы сами по себе не умирали
    nohup ./main myfile &
    pid=$!
    list_pid+=("$pid")
done

touch result.txt

# Ожидание завершения всех задач
sleep 300

# В цикле завершаем задачи
for pid in "${list_pid[@]}"
do
    kill -2 "$pid" 2>>/dev/null
done 

rm ./nohup.out
echo "All tasks completed"
