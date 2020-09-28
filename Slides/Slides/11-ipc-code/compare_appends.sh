#!/bin/bash
# Compares several approaches to atomically appending to files using
# various approaches to coordinate (or not)


echo Compiling

gcc -g -o append_clobber  append_clobber.c
gcc -g -o append_sem      append_sem.c  -lpthread
gcc -g -o append_sem_init append_sem_init.c -lpthread
gcc -g -o append_lockf    append_lockf.c
gcc -g -o append_os       append_os.c

cmds=("append_clobber" "append_sem"       # bash array of commands to run
      "append_lockf"   "append_os")

append_sem_init                           # initialize for the semaphore

nwrites=10000                             # number of writes/subprocesses 

echo Timing each command with $nwrites children
echo

for cmd in "${cmds[@]}"; do               # loop over each command
    echo $cmd
    rm -f the_file.txt
    time bash -c "for i in \$(seq $nwrites); do $cmd \$i & done"
    wc -l the_file.txt
    echo
    echo
done

