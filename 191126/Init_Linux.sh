#!/bin/bash

# input: init.in -> *.in  
#          *.cpp -> ${Title}.cpp -> *.data
# output: *.in   -> *.restart  init.log
#                -> *.lammpstrj *.log

dir=$(cd `dirname $0`;pwd)
N=30    #N of single chain
Num=10  #number of chains
Chain=${N}N_${Num}Chain

# Box size information
Box=1000.0
nBox=1.0
Segment="---------------------------------------------------------------"
Segments="\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\
\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-"

echo ${Segment}
	if [ -s ${Chain}.data ]
    then
		echo "->\"${Chain}.data\" is Already... N = ${N} ... Num = ${Num}"
	else
		echo "->\"${Chain}.data\"... N = ${N} ... Num = ${Num}"
		sed "22s/100.0/${Box}/;23s/2/${nBox}/;28s/30/${N}/;\
        29s/1/${Num}/" hybrid_chain.cpp > ${Chain}.cpp
		g++ -std=c++0x ${Chain}.cpp -o ${N}N_${Num}.chain
		chmod 740 ${N}N_${Num}.chain 
		./${N}N_${Num}.chain && rm ${N}N_${Num}.chain 
    fi 

#:<<BLOCK`
# Ellipsoids' information
for Phi in 0.48
do
 for S in  1.0           #1.0 2.0 2.5 3.0
 do
   for R in 5.0          # 0.5 1.0 1.5 2.5 5.0
   do 
    D=$(echo "$R * 2" | bc)
    
    echo ${Segment}
    Ellipsoid="${Phi}Phi_${S}S_${D}D"
	echo "->\"${Ellipsoid}.data\"...Phi = ${Phi};S = ${S};D = ${D} ..."
	sed "25s/100.0/${Box}/;26s/2/${nBox}/;37s/0.1/${Phi}/;31s/1.0/${D}/;\
    32s/2.0/${S}/;" hybrid_ellipsoid.cpp > ${Ellipsoid}.cpp
	g++ -std=c++0x ${Ellipsoid}.cpp -o ${Ellipsoid}.ellipse
	chmod 740 ${Ellipsoid}.ellipse && ./${Ellipsoid}.ellipse 
    #waiting for ${Ellipsoid}.data
    until [ -s ${Ellipsoid}.data ]
    do 
      wait 
    done
    if [ -s ${Ellipsoid}.data ]
    then 
      echo "\\\\\\\\\\\\\\\\\\\\\\\\\\\\Done.\\\\\\\\\\\\\\\\\\\\\\\\\"
    else 
      wait
    fi
    rm ${Ellipsoid}.ellipse
	mkdir ${dir}/src/
	mv ${Ellipsoid}.cpp ${dir}/src/

    Title="${Phi}Phi_${S}S_${D}D"
    echo "->\"init.in\"..."
	A=$(echo "$S * $D + 1.4" | bc)
	B=$(echo "$D + 1.4" | bc)
    a=$(echo "$S * $D" | bc)
	b=$(echo "$D" | bc)
	sed "18s/2.0/${S}/;20s/0.5/${R}/;67s/Chain/${Chain}/;68s/Ellipsoid/${Ellipsoid}/;\
    122,170s/Title/${Title}/;161s/100.0/${Box}/g" init.in > ${Ellipsoid}.in

    echo "->\"lmp_wk -i ${Ellipsoid}.in -l ${Ellipsoid}.log\"..." 
    echo ${Segment}
	lmp_wk -i ${Ellipsoid}.in -l ${Ellipsoid}.log
    sed -i "1i${Segments}\n${Ellipsoid}\n${Segments}" ${Ellipsoid}.log
    echo "
" >> ${Ellipsoid}.log
    mv ${Ellipsoid}.in ${dir}/src/ && mv ${Ellipsoid}u.lammpstrj ${dir}/src/ 
  done
 done 
done
mv *.data ${dir}/src/ && mv ${Chain}.cpp ${dir}/src/
cat *.log > log.out && mv *.log ${dir}/src/
#`BLOCK
