./main.exe 0 &
./main.exe 1 &
./main.exe 2 &
./main.exe 3 &
./main.exe 4 &
./main.exe 5 &
./main.exe 6 &
./main.exe 7 &
./main.exe 8 &
./main.exe 9 &
./main.exe 10 &
./main.exe 11 &
./main.exe 12 &
./main.exe 13 &
./main.exe 14 &
./main.exe 15 &
./main.exe 16 &
./main.exe 17 &
./main.exe 18 &
./main.exe 19 &
./main.exe 20 &
wait
cat output_256_* > output_256.dat
cat output_576_* > output_576.dat
cat output_1024_* > output_1024.dat
