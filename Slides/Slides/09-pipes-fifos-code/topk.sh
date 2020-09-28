#!/bin/bash
# 
# usage: topk.sh <K> <file>
K=$1                    # arg1 is K value
file=$2                 # arg2 is file to search

cat $file             | # Feed input \
tr -sc 'A-Za-z' '\n'  | # Translate non-alpha to newline \
tr 'A-Z' 'a-z'        | # Upper to lower case \
sort                  | # Duh \
uniq -c               | # Merge repeated, add counts \
sort -rn              | # Sort in reverse numerical order \
head -n $K              # Print only top 10 lines
