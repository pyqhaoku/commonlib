examples:examples.o pdate.o table.o pstring.o
	cc -g -Wall -O0 -o examples examples.o pdate.o table.o pstring.o -I. -lm

quicksort:quicksort.o
	cc -g -Wall -O0 -o quicksort quicksort.o

%.o:%.c
	cc -g -Wall -O0 -c $< -o $@ -I. -lm

