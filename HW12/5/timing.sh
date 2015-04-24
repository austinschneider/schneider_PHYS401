(time -p ./main 1) 2>&1 | grep real | awk '{print $2}' > timing.dat
(time -p ./main 2) 2>&1 | grep real | awk '{print $2}' >> timing.dat
(time -p ./main 3) 2>&1 | grep real | awk '{print $2}' >> timing.dat
(time -p ./main 4) 2>&1 | grep real | awk '{print $2}' >> timing.dat
