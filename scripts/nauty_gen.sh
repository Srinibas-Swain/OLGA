#!/bin/sh
for var in 3 4 5 6
do
   echo $var
   geng -c -l -s $var >> reg_gri1.g6
done
showg -A reg_gri1.g6 >> final_nauty1.txt
