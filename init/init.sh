#!/bin/bash

# for t in {0..9}

N=30
D=2.0
Box=100.0
Phi=0.1

echo "->\"chain.data\"... N = ${N} ..."
sed "22s/100.0/${Box}/;28s/30/${N}/" hybrid_chain.cpp > ${N}N_chain.cpp
g++ -std=c++11 ${N}N_chain.cpp -o ${N}N.chain
chmod 740 ${N}N.chain 
./${N}N.chain && rm ${N}N.chain ./src
#:<<BLOCK`
for S in 1.0 2.0 3.0 4.0 5.0 
do
    echo "---------------------------------------------------------------"
	echo "->\"ellipse.data\"...Phi = ${Phi};S = ${S};D = ${D} ..."
	title="${Phi}Phi_${S}S_${D}D"
	sed "23s/100.0/${Box}/;34s/0.1/${Phi}/;29s/1.0/${D}/;30s/2.0/${S}/;36s/0.1P/${Phi}P/;36s/1.0S/${S}S/;36s/2.0D/${D}D/;" hybrid_ellipsoids.cpp > ${title}.cpp
	g++ -std=c++11 ${title}.cpp -o ${title}.ellipse
	chmod 740 ${title}.ellipse && ./${title}.ellipse && rm ${title}.ellipse
	mv ${title}.cpp ./src/

    echo "->\"init.init\"..."
	a=$(echo "$S * $D + 1.2" | bc)
	b=$(echo "$D + 1.2" | bc)
	c=$(echo "$a + 0.8" | bc)
	sed "22s/1.0/${S}/;23s/5.0/${D}/;64s/30/${N}/;65,141s/0.1P/${Phi}P/;65,141s/1.0S/${S}S/;65,141s/2.0D/${D}D/;75s/1.2/$a/;75s/1.0/$b/g;84s/4.0/$c/;119s/100.0/${Box}/g" init.init > ${title}.init
    echo "->\"lmp_wk\"..." 
    echo "---------------------------------------------------------------"
	lmp_wk -i ${title}.init -l ${title}.log
    mv ${title}.init ./src/ && mv ${title}u.lammpstrj ./src/ 
  done
mv *.data ./src/ && mv *.log ./src/
mv ${N}N_chain.cpp ./src/
#`BLOCK
