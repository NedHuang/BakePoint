# Makefile build/run experiments using odds_evens_* programs and
# compare timings of variants.

# print only when lock acquired
CC = gcc -Wall -lpthread -DTHREAD_ITERS=5000 -DVPRINTF=ignore 

# # don't sleep at all
# CC = gcc -Wall -Wno-unused-value -lpthread -DTHREAD_ITERS=5000 -DVPRINTF=ignore -DSLEEPFUN=0 

# also print when failing to acquire
# CC = gcc -Wall -lpthread -DTHREAD_ITERS=5000

PROGS = busy nested_if triple_if condvar two_condvars

all : $(PROGS)

experiment : all
	for prog in $(PROGS); do echo "time $$prog:"; time $$prog > /tmp/$$prog.out; echo "-----"; done
	for prog in $(PROGS); do wc -l /tmp/$$prog.out; done

busy : odds_evens_busy.c odds_evens.h
	$(CC) -o $@ $<

condvar : odds_evens_condvar.c odds_evens.h
	$(CC) -o $@ $<

nested_if : odds_evens_busy_nested_if.c odds_evens.h
	$(CC) -o $@ $<

triple_if : odds_evens_busy_triple_if.c odds_evens.h
	$(CC) -o $@ $<

two_condvars : odds_evens_two_condvars.c odds_evens.h
	$(CC) -o $@ $<


clean :
	rm -f $(PROGS)

