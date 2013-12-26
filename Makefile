# Makefile

CC=gcc
CFLAGS=-g
OBJS=boltzmann.o image.o

boltzmann : $(OBJS)
	gcc -g -o $@ $(OBJS)
clean :
	rm -f $(OBJS)

dataset/train-labels-idx1-ubyte : 
	wget "http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz"
