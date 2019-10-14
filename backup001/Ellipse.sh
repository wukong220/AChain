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
Kb=0.0
for Phi in 0.40
do 
 for S in 1.0 2.0 2.5 3.0
 do 
  for R in 1.5 
  do 
    D=$(echo "$R * 2.0" | bc)
	for Fa in 10.0 #15.0
	do	
		title="${Phi}Phi_${S}S_${D}D"
		a=$(echo "$S * $D" | bc)
		
		if [ "$1" == "test" ]
		then
			sed "18s/2.0/$S/;142s/2.2/$S/;19,26s/0.5/$R/;38s/1.0/$Fa/;59s/100000/10000/;\
			63,134s/0.1P/${Phi}P/;63,134s/2.0S/${S}S/;63,134s/1.0D/${D}D/;\
			127s/init/test/;141s/2.0/$a/;142s/1.2/$D/g;132d" ellipse.in > ${title}.test
			mpirun -np 6 lmp_wk -i ${title}.test -l ${title}.test.log
            echo "... mpirun -np 6 lmp_wk -i ${title}.test -l ${title}.test.log ..." 
			exit 1
		else
			sed "18s/2.0/$S/;142s/2.2/$S/;19,26s/0.5/$R/;38s/1.0/$Fa/;\
			63,134s/0.1P/${Phi}P/;63,134s/2.0S/${S}S/;63,134s/1.0D/${D}D/;\
			141s/2.0/$a/;141s/1.0/$D/g;142s/1.2/$D/g;" ellipse.in > ${title}.init
			mkdir ${dir}/${title}
			mkdir ${dir}/${title}/${Kb}Kb_${Fa}Fa/
			mv ${title}.init ${dir}/${title}/${Kb}Kb_${Fa}Fa/
		        mv ${title}.restart ${dir}/${title}/${Kb}Kb_${Fa}Fa/
			cd ${dir}/${title}/${Kb}Kb_${Fa}Fa/
			nohup mpirun -np 1 lmp_wk -i ${title}.init -l ${title}.init.log > ${title}.init.out &
            echo "... nohup mpirun -np 1 lmp_wk -i ${title}.init -l ${title}.init.log >  ${title}.init.out & ..."
			cd ../../
		fi
      done
  done
 done
done
