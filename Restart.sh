#!/bin/bash

# for t in {0..9}
#for t in 63 225 405 810 1800 2700 7200 9000 19800 35100
#do
#    cd  ~/wukong/chain
#    bsub  -q gtian -R "span[ptile=8]" -n 8 mpich2-mpiexec -np 8 lmp_wk"<"0.07_$t.in">"0.07_$t.data
#    cd ..
#done 

mpirun -np 6 lmp_wk -i restart.in -l restart.log
