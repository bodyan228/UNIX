#Удаление архивов, если есть, перед запуском программы
rm -rf ./*.gz 2>>/dev/null

#Компилирование файла программы и файла генерации A_FILE
make gen 1>>//dev/null
make program 1>>/dev/null

#Создание файла
./make_file 

#Создание sparse-файла B_FILE на основе A_FILE 
./sparse -iA_FILE -oB_FILE

#Сжатие файлов
gzip -k A_FILE
gzip -k B_FILE

#Распковка файла B_FILE в stdout и перенаправление в stdin программе
gzip -cd B_FILE.gz | ./sparse -oC_FILE

#Создание sparse-файла D_FILE на основе A_FILE с размером блока 100 байт
./sparse -iA_FILE -oD_FILE -b100

#Удаление скомпилированных файлов
rm make_file sparse

#Запись stat'ов в файл result.txt
stat A_FILE > result.txt
echo "----------------------------------------------------------------------" >> result.txt
stat A_FILE.gz >> result.txt
echo "----------------------------------------------------------------------" >> result.txt
stat B_FILE >> result.txt 
echo "----------------------------------------------------------------------" >> result.txt
stat B_FILE.gz >> result.txt
echo "----------------------------------------------------------------------" >> result.txt
stat C_FILE >> result.txt
echo "----------------------------------------------------------------------" >> result.txt
stat D_FILE >> result.txt