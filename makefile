pdate:pdate.o
	cc -g -Wall -O0 -o pdate pdate.o

%.o:%.c
	cc -g -Wall -O0 -c $< -o $@

