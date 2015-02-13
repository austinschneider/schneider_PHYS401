# Compile
make

# Run
./relax.exe 10 >&  relax.dat
make paper
mv figure.pdf figure_10.pdf
./relax.exe 1000 >& relax.dat
make paper
mv figure.pdf figure_1000.pdf

