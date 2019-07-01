#!/bin/bash

# for t in {0..9}

N=30
R=2.0
Box=100.0
Phi=0.1

#chain.data
sed "22s/100.0/${Box}/;28s/30/${N}/" hybrid_chain.cpp > ${N}N_chain.cpp
g++ -std=c++11 ${N}N_chain.cpp -o ${N}N.chain
chmod 740 ${N}N_chain.cpp && chmod 740 ${N}N.chain 
./${N}N.chain && rm ${N}N.chain 
rm ${N}N_chain.cpp

:<<BLOCK`	
for S in 1.0  #2.0 3.0 4.0 5.0 
do
	#ellipse.data
	title="${Phi}Phi_${S}S_${R}R"
	sed "23s/100.0/${Box}/;34s/0.1/${Phi}/;29s/1.0/${R}/;30s/2.0/${S}/;36s/0.1/${Phi}/;36s/1.0/${S}/;36s/2.0/${R}/;" hybrid_ellipsoids.cpp > ${title}.cpp
	g++ -std=c++11 ${title}.cpp -o ${title}.ellipse
	chmod 740 ${title}.ellipse && ./${title}.ellipse
	rm ${title}.cpp && rm ${title}.ellipse
done

	a=$(echo "$S * $R + 1.2" | bc)
	b=$(echo "$R + 1.2" | bc)
	c=$(echo "$a + 0.8" | bc)
	sed "64s/30/${N}/;65,141s/1.0S/${S}S/;65,141s/2.0R/${R}R/;75s/1.2/$a/;75s/1.0/$b/g;84s/4.0/$c/;119s/100.0/${Box}/g" 0.1Phi.init > ${title}.init
	chmod 740 ${title}.init
	lmp_wk -i ${title}.init
	
done
`BLOCK