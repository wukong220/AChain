#!/bin/bash

# input:  00*.init.restart 
# output: 00*.init_chain.lammpstrj 
#         00*.lammpstrj 00*u.lammpstrj 00*f.lammpstrj
#         00*.a.restart 00*.b.restart  00*.end.restart
#         00*_msd.txt   00*.log        00*.data

dir=$(cd `dirname $0`;pwd)
N_init=5         #number of asembles
#run=1000000000

for Phi in 0.48 
do 
 for S in 1.0   #2.0 2.5 3.0           # 2.0 3.0 4.0 5.0 #4.0
 do 
  for R in 5.0 
  do 
    D=$(echo "$R * 2.0" | bc)
    Title="${Phi}Phi_${S}S_${D}D"
    echo ${Title}
    a=$(echo "$S * $D" | bc)
  for Kb in 1.05 3.0 5.0
    do 
      for Fa in 0.5    #15.0
      do  
        if [ "$1" == "test" ]
        then
          sed "21s/2.0/${S}/;23s/0.5/${R}/;40s/1.0/${Kb}/;42s/1.0/${Fa}/;\
          74,181s/001.init/${Title}/g;116s/init/test/;160,182d" act.in > ${Title}.test.act
          cp ${Title}.test.act ${dir}/${Title} && cd  ${dir}/${Title}
          mpirun -np 1 lmp_wk -l ${Title}.test_act.log -i ${Title}.test.act
          echo "... mpirun -np 1 lmp_wk -l ${Title}.test_act.log -i ${Title}.test.act ..."
          cd ../
          exit 1
        else
            #mkdir ${dir}/${Title}/
            #mkdir ${dir}/${Title}/${Kb}Kb_${Fa}Fa/
            #mv ${t}.init.restart ${dir}/${Title}/${Kb}Kb_${Fa}Fa/
          for ((t=1;t<=${N_init};t++))
          do 
            if ((t<10))
            then
              sed "21s/2.0/${S}/;23s/0.5/${R}/;40s/1.0/${Kb}/;42s/1.0/${Fa}/;\
              74,181s/001/00$t/g" act.in > 00${t}.act 
              mv 00${t}.act ${dir}/${Title}/${Kb}Kb_${Fa}Fa/
              cd ${dir}/${Title}/${Kb}Kb_${Fa}Fa
              bsub -J ${Phi}${D}_$t -q serial -n 1 -i 00$t.act -o 00$t.act.output\
              -e 00$t.act.err mpirun lmp_wk -l 00$t.act.log > 00$t.act.data
              echo " --- bsub -J ${Phi}${D}_$t -q serial -n 1 -i 00$t.act -o 00$t.act.output\
              -e 00$t.act.err mpirun lmp_wk -l 00$t.act.log > 00$t.act.data  --- "                                      #NJU
              
              #sub -J ${Phi}${D}_00$t -q e5645! -n 1 -i 00$t.act -o 00$t.act.output\
              #-e 00$t.act.err mpirun lmp_wk -l 00$t.act.log > 00$t.act.data
              
              #echo "sub -J ${Phi}${D}_00$t -q e5645! -n 1 -i 00$t.act -o 00$t.act.output\
              #-e 00$t.act.err mpirun lmp_wk -l 00$t.act.log > 00$t.act.data"
              
              #mpirun -np 4 lmp_wk -l 00$t.log -i 00$t.act 
              #echo "mpirun -np 4 lmp_wk -l 00$t.log -i 00$t.act"
            elif ((t<100)) 
            then 
              sed "21s/2.0/${S}/;23s/0.5/${R}/;40s/1.0/${Kb}/;42s/1.0/${Fa}/;\
              74,181s/001/0$t/g" act.in > 0${t}.act 
              mv 0${t}.act ${dir}/${Title}/${Kb}Kb_${Fa}Fa/
              cd ${dir}/${Title}/${Kb}Kb_${Fa}Fa
              bsub -J ${Phi}${D}_$t -q serial -n 1 -i 0$t.act -o 0$t.act.output\
              -e 0$t.act.err mpirun lmp_wk -l 0$t.act.log > 0$t.act.data    
              echo " --- bsub -J ${Phi}${D}_$t -q serial -n 1 -i 0$t.act -o 0$t.act.output\
              -e 0$t.act.err mpirun lmp_wk -l 0$t.act.log > 0$t.act.data --- "                                 #NJU
              
              #sub -J ${Phi}${D}_0$t -q e5645! -n 1 -i 0$t.act -o 0$t.act.output\
              #-e 0$t.act.err mpirun lmp_wk -l 0$t.act.log > 0$t.act.data    
              #echo " --- bsub -J ${Phi}${D}_0$t -q e5645! -n 1 -i 0$t.act -o 0$t.act.output\
              #-e 0$t.act.err mpirun lmp_wk -l 0$t.act.log > 0$t.act.data --- "                
              
              #nohup mpirun -np 1 lmp_wk -l $t.log -i $t.act.in > $t.data &
              #echo "nohup mpirun -np 1 lmp_wk -l $t.log -i $t.act.in > $t.data &"
            else 
              echo "////////////// ERROR!! N_init is more than 100 ///////////////////"
              exit 1
            fi 
            cd ../../ 
          done 
        fi
      done  
    done
  done
 done
done


#: <<`COMMENT`
# for t in {0..9}
#for t in 001 002 003 004 005 006 007 008 009 010 011 012 013 014 015 016 017 018 019 020
#do
#    cd  ~/wukong/chain
#    bsub  -q gtian -R "span[ptile=8]" -n 8 mpich2-mpiexec -np 8 lmp_wk"<"0.07_$t.in">"0.07_$t.data
#    cd ..
#done 

#for i in 001 002 003           # 004 005 006 007 008 009 010
#do 
   #  sed "59,194s/001/$i/g" test.in > $i.act.in 

#     bsub -J $i -n 1 mpirun lmp_wk -l $i.log -i $i.act.in > $i.data                                      #NJU
#     echo "bsub -J $i -n 1 mpirun lmp_wk -l $i.log -i $i.act.in > $i.data"
   
     #bsub  -q gchen -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$i.act.in">"$i.data            #gchen
     #echo "bsub  -q gchen -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$i.act.in">"$i.data"

     #bsub  -q gtian -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$i.act.in">"$i.data            #gtian
     #echo "bsub  -q gtian -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$i.act.in">"$i.data"

  #   nohup mpirun -np 1 lmp_wk -l $i.log -i $i.act.in > $i.data &                                        #nohup
 #    echo "nohup mpirun -np 1 lmp_wk -l $i.log -i $i.act.in > $i.data &"
#done 

#for t in 001 002 003 004 005 006 007 008 009 010 011 012 013 014 015 016 017 018 019 020 
#do
     #sed "59,194s/001/$t/g" test.in > $t.act.in 

     #bsub -J $t -n 1 mpirun lmp_wk -l $t.log -i $t.act.in > $t.data                                      #NJU
     #echo "bsub -J $t -n 1 mpirun lmp_wk -l $t.log -i $t.act.in > $t.data"
   
     #bsub  -q gchen -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$t.act.in">"$t.data            #gchen
     #echo "bsub  -q gchen -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$t.act.in">"$t.data"

     #bsub  -q gtian -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$t.act.in">"$t.data            #gtian
     #echo "bsub  -q gtian -R "span[ptile=1]" -n 1 mpich2-mpiexec -np 1 lmp_wk"<"$t.act.in">"$t.data"

     #nohup mpirun -np 1 lmp_wk -l $t.log -i $t.act.in > $t.data &                                        #nohup
     #echo "nohup mpirun -np 1 lmp_wk -l $t.log -i $t.act.in > $t.data &"
#done
#COMMENT
