#!/bin/bash

# for t in {0..9}
#for t in 63 225 405 810 1800 2700 7200 9000 19800 35100
#do
#    cd  ~/wukong/chain
#    bsub  -q gtian -R "span[ptile=8]" -n 8 mpich2-mpiexec -np 8 lmp_wk"<"0.07_$t.in">"0.07_$t.data
#    cd ..
#done 

for i in 001 002 003 004 005 006 007 008
do 
 sed "59,184s/001/$t/g" test.in > $t.act.in 
 bsub -J $t -n 1 mpirun lmp_wk -l $t.log -i $t.act.in > $t.data
 echo "bsub -J $t -n 1 mpirun lmp_wk -l $t.log -i $t.act.in > $t.data"
done 

for t in 009 010 011 012 013 014 015 016 017 018 019 020 
do
     sed "59,184s/001/$t/g" test.in > $t.act.in 
   # mpirun -np 4 lmp_wk -l $t.log -i $t.act.in
     #bsub -J $t -n 1 mpirun lmp_wk -l $t.log -i $t.act.in > $t.data
     #echo "bsub -J $t -n 1 mpirun lmp_wk -l $t.log -i $t.act.in > $t.data"
     bsub  -q gchen -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$t.act.in">"$t.data
    echo "bsub  -q gchen -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$t.act.in">"$t.data"
done

