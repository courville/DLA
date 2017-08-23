#!/bin/sh

DATE=`date +%Y%m%d%H%M`
touch res-${DATE}.log
tail -f res-${DATE}.log &
for s in 64 128 256 512 1024 2048 4096
do
  for i in `seq -f "%02g" 1 9`
  do
    #echo dla s $s m1 10 m2 0 h 1 iteration $i
    ./dla $s 10 0 1 "${s}-${i}" | grep DLA >> res-${DATE}.log &
  done
  i=10
  #echo dla s $s m1 10 m2 0 h 1 iteration $i
  ./dla $s 10 0 1 "${s}-${i}" | grep DLA >> res-${DATE}.log
done

echo "s,nbpart,dim" > res-${DATE}.log
cat res-${DATE}.log | grep number | sed 's/^.*s=\([0-9]*\),.*parts \([0-9]*\) .*dimension \([0-9\.]*\)$/\1,\2,\3/g' >> res-${DATE}.csv

for infile in *.pgm
do
  base=`basename ${infile} .pgm`
  [ ! -f ${base}.png ] && convert ${base}.pgm ${base}.png
done
