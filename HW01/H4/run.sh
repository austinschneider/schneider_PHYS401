echo "" > output.dat
echo "" > timing.dat
run () {
    echo "Number of samples: ${1}" >> output.dat
    (time -p ./jackknife.exe random_numbers_s2015_c${1}.dat >> output.dat) 2>&1 | grep real | awk '{print var " " $2}' var="$1" >> timing.dat;
}

echo # size time
for i in 1000 10000000 20000000 30000000 40000000 50000000 60000000 70000000 80000000
do
    run $i
done

