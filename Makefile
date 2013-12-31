# Makefile
CC=gcc
CFLAGS=-g
OBJS=boltzmann.o image.o net.o
DATAS=dataset/train-labels-idx1-ubyte dataset/train-images-idx3-ubyte dataset/t10k-labels-idx1-ubyte dataset/t10k-images-idx3-ubyte

datafiles:=$(wildcard dataset/*)
gzfiles=$(xmlfiles:$=.gz)


boltzmann : $(OBJS) $(DATAS)
	gcc -g -o $@ $(OBJS)
clean :
	rm -f $(OBJS)

# http://yann.lecun.com/exdb/mnist/


dataset/train-labels-idx1-ubyte : 
	wget "http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz" -P dataset/
	gunzip -c dataset/train-labels-idx1-ubyte.gz > $@

dataset/train-images-idx3-ubyte : 
	wget "http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz" -P dataset/
	gunzip -c dataset/train-images-idx3-ubyte.gz > $@

dataset/t10k-labels-idx1-ubyte : 
	wget "http://yann.lecun.com/exdb/mnist/t10k-labels-idx1-ubyte.gz" -P dataset/
	gunzip -c dataset/t10k-labels-idx1-ubyte.gz > $@

dataset/t10k-images-idx3-ubyte : 
	wget "http://yann.lecun.com/exdb/mnist/t10k-images-idx3-ubyte.gz" -P dataset/
	gunzip -c dataset/t10k-images-idx3-ubyte.gz > $@
