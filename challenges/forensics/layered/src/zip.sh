#!/bin/bash
for x in {0..1000}
do
n="challenge$x.zip" 

if [ $x -eq 0 ]
then
zip $n "challengeFinal.zip"
else
n1="challenge$((x-1)).zip"
echo n1
zip $n $n1
fi

if [ $x -ne 0 ]
then
i=$((x-1))
n="challenge$i.zip"
rm $n  

fi
done
