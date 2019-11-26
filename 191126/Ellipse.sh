#!/bin/bash

# input: ellipse.in **.restart
# run: lmp_wk
# output: **.in **.restart **.log 

dir=$(cd `dirname $0`;pwd)
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"

N_init=5   #number of initialization 

for Phi in 0.48
do 
  for S in 1.0 # 2.0 2.5 3.0
  do  
    for R in 5.0 
    do 
      D=$(echo "$R * 2.0" | bc)
      Ellipsoid="${Phi}Phi_${S}S_${D}D"
      Title="${Phi}Phi_${S}S_${D}D"
      echo ${Title}
      a=$(echo "$S * $D" | bc)
          
      if [ "$1" == "test" ]
      then
      	sed "21s/2.0/$S/;23s/0.5/$R/;64s/100000/10000/;67s/5/${N_init}/;\
      	70,154s/Title/${Title}/;130,213s/init/test/" ellipse.in > ${Ellipsoid}.test.init
        cp ${Ellipsoid}.test.init ${dir}/${Ellipsoid}/ && cp ${Ellipsoid}.restart  ${dir}/${Ellipsoid}/
      	cd  ${dir}/${Ellipsoid}/
        mpirun -np 6 lmp_wk -i ${Ellipsoid}.test.init -l ${Ellipsoid}.test_init.log
        echo "... mpirun -np 6 lmp_wk -i ${Ellipsoid}.test.init -l ${Ellipsoid}.test_init.log ..." 
        cd ../
      	exit 1
      else 
        sed "21s/2.0/$S/;23s/0.5/$R/;67s/5/${N_init}/;70,154s/Title/${Title}/;"\
          	ellipse.in > ${Ellipsoid}.init
        #make directories
      	mkdir ${dir}/${Ellipsoid}
      	cp ${Ellipsoid}.init ${dir}/${Ellipsoid}/ && cp ${Ellipsoid}.restart ${dir}/${Ellipsoid}/
      	cd ${dir}/${Ellipsoid}/
        
        #bsub -J ${Title} -q serial -n 1 -i ${Ellipsoid}.init -o ${Title}.output -e ${Title}.init.err mpirun lmp_wk -l ${Ellipsoid}.init.log 
        #echo "... bsub -J ${Title} -q serial -n 1 -i ${Ellipsoid}.init -o ${Title}.output -e ${Title}.err mpirun lmp_wk -l ${Ellipsoid}.init.log ..."
                
        #sub -J ${Title} -q e5645! -n 1 -i ${Ellipsoid}.init -o ${Title}.output -e ${Title}.init.err mpirun lmp_wk -l ${Ellipsoid}.init.log
        #echo "... sub -J ${Title} -q e5645! -n 1 -i ${Ellipsoid}.init -o ${Title}.output -e ${Title}.err mpirun lmp_wk -l ${Ellipsoid}.init.log ..."
        
        #Elipse initialization     
        for Kb in 1.05 3.0 5.0
        do  
      	  for Fa in 0.5 #15.0
      	  do 	
            mpirun -np 4 lmp_wk -i ${Ellipsoid}.init -l ${Ellipsoid}.init.log  
            echo "... mpirun -np 4 lmp_wk -i ${Ellipsoid}.init -l ${Ellipsoid}.init.log ..."
            
            #rename
            for ((i=1;i<=${N_init};i++))
            do 
              if ((i<10))
              then
                mv init.${i}00000.restart 00$i.init.restart
              elif ((i<100)) 
              then 
                mv init.${i}00000.restart 0$i.init.restart
              else 
                echo "////////////// ERROR!! N_init is more than 100 ///////////////////"
                exit 1
              fi 
              mkdir ${dir}/${Ellipsoid}/${Kb}Kb_${Fa}Fa/
              mv *.init.restart ${dir}/${Ellipsoid}/${Kb}Kb_${Fa}Fa/
            done  
          done
        done
        
       cd ../
      fi
    
    done
  done 
done 
