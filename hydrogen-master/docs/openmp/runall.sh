rm *.out
ls *cc | sed 's/\(.*\)\.\([a-z][a-z]*\)/\1/' | xargs -I file clang++ -std=c++11 file.cc -fopenmp -o file.out
ls *cc | sed 's/\(.*\)\.\([a-z][a-z]*\)/\1/' | xargs -I file `./file`

