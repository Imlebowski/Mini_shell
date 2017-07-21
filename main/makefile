LFLAG := -L ../lib -lshell
IFLAG := -I ../include
SRCS := $(wildcard *.c)
TARGET := $(SRCS:.c=)

all : ${TARGET}
% : %.c
	gcc $^ ${LFLAG} ${IFLAG} -o $@

clean :
	rm ${TARGET}
