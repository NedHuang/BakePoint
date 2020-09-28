#!/bin/bash
for word in $(cat 1000_words.txt); do
    shmdemo $word &
done
