#!/bin/bash

file="main.cpp"
# for t in {0..9}

dir=$(cd `dirname $0`;pwd)
for Phi in 0.30 
do 
 for S in 1.0     #2.0 2.5 3.0    #2.0 2.5 3.0           # 2.0 3.0 4.0 5.0 #4.0
 do 
  for R in 1.0 
  do 
    D=$(echo "$R * 2.0" | bc)
   for Fa in 10.0    #15.0
   do
    for t in 10   # 15 30             # 01 02 03 10 15 30  
    do 
       title="${Phi}Phi_${S}S_${D}D"
       echo ${title}
       sed -e "16,19s/0.40/0$t/g" $file  > 0$t.msd.cpp
       #sed -e "10s/1/$t/g" -e "17,19s/001/0$t/g" $file  > 0$t.msd.cpp
       g++ -std=c++0x paras.cpp files.cpp 0$t.msd.cpp -o 0$t.msd      #for NJU
#  g++ -std=c++11 *.cpp -o $t.msd        #for Mac
       nohup ./0$t.msd > 0$t.output &
    done 
    done 
   done 
 done 
done
