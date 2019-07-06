#!/bin/sh

# for t in {0..9}
for t in 011 012 013 014 015 016 017 018 019 020 #001 002 003 004 005 006 007 008 009 010
         #011 012 013 014 015 016 017 018 019 020
do
    sed "64,103s/001/$t/" test.in > $t.test.testin
    lmp_wk -i $t.test.testin -l $t.test.log 
  done
