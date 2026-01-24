#!/usr/bin/env bash
set -euo pipefail

NTESTS=1000
for ((t=1;t<=NTESTS;t++)); do
  n=$(( (RANDOM % 12) + 1 ))
  a=""
  for ((i=0;i<n;i++)); do
    v=$(( ((RANDOM<<15)|RANDOM) & ((1<<20)-1) ))
    # append
    a+="$v"
    if [ $i -ne $((n-1)) ]; then a+=" "; fi
  done
  inp="$n\n$a\n"
  out1=$(printf "%b" "$inp" | ./main)
  out2=$(printf "%b" "$inp" | ./brute)
  if [ "$out1" != "$out2" ]; then
    echo "Mismatch on test $t"
    echo "n= $n"
    echo "a= $a"
    echo "main: $out1"
    echo "brute: $out2"
    exit 1
  fi
done

echo "All $NTESTS random tests passed"

