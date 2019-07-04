#!/bin/sh

# for t in {0..9}
sed "141s/4.0/2.0/;141s/2.0/3.0/g" restart.in > restart.test